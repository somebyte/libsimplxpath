#ifndef __SIMPLXPATH_IMPL_HPP__
#define __SIMPLXPATH_IMPL_HPP__

#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <memory>


namespace simplxpath
{
  class xmldoc_impl;
  class xpath_object_impl;
  class element_impl;
  class attribute_impl;

  typedef std::shared_ptr< xmldoc_impl       > xmldoc_impl_ptr;
  typedef std::shared_ptr< xpath_object_impl > xpath_object_impl_ptr;
  typedef std::shared_ptr< element_impl      > element_impl_ptr;
  typedef std::shared_ptr< attribute_impl    > attribute_impl_ptr;
   
  //------------------------------------------------------------------------
  class xmldoc_impl
  {
  public:
    explicit xmldoc_impl (const std::string& filename);
             xmldoc_impl (const std::string& xmldump, size_t dumpsize);
            ~xmldoc_impl ();
       
          bool                  valid      () const;
    const std::string&          filename   () const;
          xpath_object_impl_ptr operator[] (const std::string& expression);
          void                  save       (const std::string& filename = "");
    
  private:        
        xmldoc_impl (const xmldoc_impl&);
        const xmldoc_impl& operator= (const xmldoc_impl&);

        std::string        _filename;
        xmlDocPtr          _doc;
        xmlXPathContextPtr _context;
  };
    
  //------------------------------------------------------------------------
  class xpath_object_impl
  {
  public:
    explicit xpath_object_impl (xmlXPathObjectPtr  object,
                                const std::string& expression);
            ~xpath_object_impl ();
        
          size_t           size       () const;
          bool             empty      () const;    
    const std::string&     expression () const;
          element_impl_ptr operator[] (size_t number);

  private:
    xpath_object_impl (const xpath_object_impl&);
    const xpath_object_impl& operator= (const xpath_object_impl&);
        
    typedef std::vector< element_impl_ptr > elements;
        
    std::string _expression;
    elements    _elements;
  };
    
  //------------------------------------------------------------------------
  class element_impl
  {
  public:
    explicit element_impl (xmlNodePtr ptr);
            ~element_impl ();

    const std::string&       name      () const;
    const std::string&       content   () const;
          void               content   (const std::string& text);
          attribute_impl_ptr attribute (const std::string& name);
          element_impl_ptr   new_element   (const std::string& name,
                                            const std::string& content);
          attribute_impl_ptr new_attribute (const std::string& name,
                                            const std::string& value );
                                               
  private:
    element_impl (const element_impl&);
    const element_impl& operator= (const element_impl&);
        
    typedef std::list< element_impl_ptr > elements;
    typedef std::map < std::string, attribute_impl_ptr > attributes;
        
    xmlNodePtr  _node;
    std::string _name;
    std::string _content;
    attributes  _attributes;
    elements    _new_elements;
  };
    
  //------------------------------------------------------------------------
  class attribute_impl
  {
  private:
  public:
        explicit attribute_impl (xmlNodePtr ptr, const std::string& name);
                ~attribute_impl ();
        
        const std::string& name  () const;
        const std::string& value () const;
              void         value (const std::string& text);
        
  private:
    attribute_impl (const attribute_impl&);
    const attribute_impl& operator= (const attribute_impl&);
        
    xmlNodePtr  _node;
    std::string _name;
    std::string _value;
  };

} // simplxpath

#endif // __SIMPLXPATH_IMPL_HPP__

