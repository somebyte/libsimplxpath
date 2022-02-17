#include <iostream>
#include "simplxpath_impl.hpp"

using std::string;
using std::nothrow;
using std::cerr;
using std::endl;

namespace simplxpath
{
  //------------------------------------------------------------------------
  // xmldoc_impl
  //------------------------------------------------------------------------
    
  //------------------------------------------------------------------------
  xmldoc_impl::xmldoc_impl (const string& filename)
  : _filename (filename),
    _doc      (nullptr),
    _context  (nullptr)
  {
    _doc = xmlParseFile (_filename.c_str());
    if (!_doc)
      {
        cerr << "Error: unable to parse file \"" << _filename << "\"" << endl;
        return;
      }
        
    _context = xmlXPathNewContext (_doc);
    if (!_context)
      {
        cerr << "Error: unable to create new XPath context" << endl;
        return;
      }
  }
    
  //------------------------------------------------------------------------
  xmldoc_impl::xmldoc_impl (const string& dump, size_t dumpsize)
  : _doc     (nullptr),
    _context (nullptr)
  {
    if (!dumpsize)
      {
        dumpsize = dump.size();
      }
        
    _doc = xmlReadMemory( dump.c_str(), dumpsize, "", nullptr, 0 );
        
    if (!_doc)
      {
        cerr << "Error: unable to read memory dump" << endl;
        return;
      }
        
    _context = xmlXPathNewContext( _doc );
        
    if (!_context)
      {
        cerr << "Error: unable to create new XPath context" << endl;
        return;
      }
  }
    
  //------------------------------------------------------------------------
  xmldoc_impl::~xmldoc_impl ()
  {
    xmlXPathFreeContext (_context);
    xmlFreeDoc (_doc);
  }
    
  //------------------------------------------------------------------------
  bool xmldoc_impl::valid () const
  {
    return  _doc && _context;
  }

  //------------------------------------------------------------------------
  const string& xmldoc_impl::filename () const
  {
    return _filename;
  }
    
  //------------------------------------------------------------------------
  xpath_object_impl_ptr xmldoc_impl::operator[] (const string& expression)
  {
    xmlXPathObjectPtr object = xmlXPathEvalExpression (
                                                BAD_CAST expression.c_str(),
                                                                  _context);
        
    if (!object)
      {
        cerr << "Error: unable to evaluate expression \""
             << expression
             << "\"" << endl;

        return nullptr;
      }
            
    xpath_object_impl_ptr ptr (new (nothrow)
                                   xpath_object_impl (object, expression));
        
    xmlXPathFreeObject (object);

    return ptr;
  }

  //------------------------------------------------------------------------
  void xmldoc_impl::save (const std::string& filename)
  {
    if (filename.empty())
      {
        if (_filename.empty())
          {
            _filename = "default.xml";
          }

         xmlSaveFile( _filename.c_str(), _doc );
         return;
      }
        
    xmlSaveFile( filename.c_str(), _doc );
    _filename = filename;
  }
    
  //------------------------------------------------------------------------
  // xpath_object_impl
  //------------------------------------------------------------------------

  //------------------------------------------------------------------------
  xpath_object_impl::xpath_object_impl (xmlXPathObjectPtr object,
                                        const string& expr)
  : _expression (expr)
  {
    if (!object)
      {
        return;
      }
        
    for (int i = 0;
             i < ( object->nodesetval ? object->nodesetval->nodeNr : 0 );
           ++i)
      {
        xmlNodePtr node = object->nodesetval->nodeTab[i];
            
        if (node->type == XML_ELEMENT_NODE)
          {
            element_impl_ptr ptr (new (nothrow) element_impl (node));
          
            if (ptr)
              {
                 _elements.push_back (ptr);
              }
          }
      }        
  }
    
  //------------------------------------------------------------------------
  xpath_object_impl::~xpath_object_impl()
  {
  }
    
  //------------------------------------------------------------------------
  size_t xpath_object_impl::size() const
  {
    return _elements.size();
  }
    
  //------------------------------------------------------------------------
  bool xpath_object_impl::empty() const
  {
    return _elements.empty();
  }
    
  //------------------------------------------------------------------------
  const string& xpath_object_impl::expression() const
  {
    return _expression;
  }
    
  //------------------------------------------------------------------------
  element_impl_ptr xpath_object_impl::operator[] (size_t number)
  {
    return _elements.at (number);
  }
    
  //------------------------------------------------------------------------
  // element_impl
  //------------------------------------------------------------------------
    
  //------------------------------------------------------------------------
  element_impl::element_impl (xmlNodePtr node)
  : _node(node)
  {
    if (!node)
      {
        return;
      }
                
    const xmlChar *name = node->name;        
        
    _name =  (const char *) (name ? name : (BAD_CAST ""));
        
        
    xmlChar *content = xmlNodeGetContent (node);
        
    _content = (const char *) (content ? content : (BAD_CAST ""));
        
    xmlFree (content);
  }
    
  //------------------------------------------------------------------------
  element_impl::~element_impl()
  {
  }
    
  //------------------------------------------------------------------------
  const string &element_impl::name() const
  {
    return _name;
  }
    
  //------------------------------------------------------------------------
  const string &element_impl::content() const
  {
    return _content;
  }
    
  //------------------------------------------------------------------------
  void element_impl::content (const string& text)
  {
    if (_node)
      {
        _content = text;
        xmlNodeSetContent (_node, (const xmlChar*) _content.c_str());
      }
  }
    
  //------------------------------------------------------------------------
  attribute_impl_ptr element_impl::attribute( const string& name )
  {
    if (!_node)
      return nullptr;
        
    attributes::iterator itr = _attributes.find (name);
        
    if (itr == _attributes.end())
      {
        xmlAttrPtr ptr = xmlHasProp ( _node, (const xmlChar *) name.c_str());
            
        if (ptr)
          {
            attribute_impl_ptr p (new (nothrow) attribute_impl (_node, name));

            if (p)
              {
                _attributes [name] = p;
                return p;
              }
          }
            
        return nullptr;
      }
        
    return itr->second;
  }
    
  //------------------------------------------------------------------------
  element_impl_ptr element_impl::new_element (const string& name,
                                              const string& content)
  {
    if (!_node)
      return nullptr;
        
    xmlNodePtr ptr = xmlNewChild (_node, nullptr, (const xmlChar *) name.c_str(),
                                               (const xmlChar *) content.c_str());
        
    if (ptr)
      {
        element_impl_ptr p (new (nothrow) element_impl (ptr));
            
        if (p)
          {
            _new_elements.push_back (p);
            return p;
          }
      }
        
    return nullptr;
  }
    
  //------------------------------------------------------------------------
  attribute_impl_ptr element_impl::new_attribute (const string& name,
                                                  const string& content)
  {
    if (!_node)
      return nullptr;
        
    xmlAttrPtr ptr = xmlHasProp (_node, (const xmlChar *) name.c_str());
        
    if (!ptr)
      {
        ptr = xmlNewProp (_node, (const xmlChar *) name.c_str(),
                                 (const xmlChar *) content.c_str());
            
        if (!ptr)
          return nullptr;
                
        attribute_impl_ptr p (new attribute_impl (_node, name));
            
        if (p)
          {
            _attributes[name] = p;
            return p;
          }
      }
    else
      {
        attribute_impl_ptr attr = attribute (name);
        attr->value (content);
        return attr;
      }

    return nullptr;
  }
    
  //------------------------------------------------------------------------
  // attribute_impl
  //------------------------------------------------------------------------
    
  //------------------------------------------------------------------------
  attribute_impl::attribute_impl (xmlNodePtr ptr, const string& name)
  : _node (ptr),
    _name (name)
  {
    if (_node)
      {
        const xmlChar *str = xmlGetProp (_node, (const xmlChar*) name.c_str());
        _value = (const char *) (str ? str : (const xmlChar*) "");
      }
  }
    
  //------------------------------------------------------------------------
  attribute_impl::~attribute_impl()
  {
  }
    
  //------------------------------------------------------------------------
  const string &attribute_impl::name() const
  {
    return _name;
  }
    
  //------------------------------------------------------------------------
  const string &attribute_impl::value() const
  {
    return _value;
  }
    
  //------------------------------------------------------------------------
  void attribute_impl::value( const string& value )
  {
    if (!_node)
      return;
            
    _value = value;
    xmlSetProp (_node, (const xmlChar *) _name.c_str(), (const xmlChar *) _value.c_str());
  }   
} // simplxpath

