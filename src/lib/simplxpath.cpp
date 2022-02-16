#include "simplxpath_impl.hpp"
#include <simplxpath.hpp>
#include <stdexcept>
#include <iostream>

namespace simplxpath
{
  using std::nothrow;
  using std::string;
  using std::out_of_range;
  using std::shared_ptr;
  using std::cerr;
  using std::endl;
    
  static const string empty ("");

  //------------------------------------------------------------------------
  // xmldoc
  //------------------------------------------------------------------------

  //------------------------------------------------------------------------
  xmldoc::xmldoc (const xmldoc& from)
  : _impl (from._impl) 
  {
  }
   
  //------------------------------------------------------------------------
  xmldoc::xmldoc (const string& filename)
  : _impl (new (nothrow) xmldoc_impl (filename)) 
  {
  }
    
  //------------------------------------------------------------------------
  xmldoc::xmldoc (const string& dump, size_t dumpsize)
  : _impl (new (nothrow) xmldoc_impl (dump, dumpsize))
  {
  }
    
  //------------------------------------------------------------------------
  xmldoc::~xmldoc()
  {
  }

  //------------------------------------------------------------------------
  xmldoc& xmldoc::operator= (const xmldoc& from)
  {
    if (this != &from)
      {
        _impl = from._impl;
      }

    return *this;
  }

  //------------------------------------------------------------------------
  bool xmldoc::valid() const
  {
    return _impl.get() ? _impl->valid() : false;
  }

  //------------------------------------------------------------------------
  const string& xmldoc::filename() const
  {
    return _impl.get() ? _impl->filename() : empty;
  }

  //------------------------------------------------------------------------
  xpath_object xmldoc::operator[] (const string& expression)
  {
    if (_impl.get())
      {
        return xpath_object ((*_impl)[expression]);
      }
      
    return xpath_object (nullptr);
  }

  //------------------------------------------------------------------------
  void xmldoc::save (const string& filename)
  {
    if (_impl.get())
      {
        _impl->save (filename);
      }
  }

  //------------------------------------------------------------------------
  // xpath_object
  //------------------------------------------------------------------------

  //------------------------------------------------------------------------
  xpath_object::xpath_object (const xpath_object& from)
  : _impl (from._impl)
  {
  }

  //------------------------------------------------------------------------
  xpath_object::xpath_object (const xpath_object_impl_ptr& ptr)
  : _impl (ptr)
  {
  }

  //------------------------------------------------------------------------
  xpath_object::~xpath_object()
  {
  }

  //------------------------------------------------------------------------
  const xpath_object& xpath_object::operator=  (const xpath_object& from)
  {
    if (this != &from)
      {
        _impl = from._impl;
      }

    return *this;
  }

  //------------------------------------------------------------------------
  size_t xpath_object::size() const
  {
    if (_impl)
      {
        return _impl->size();
      }
      
    return 0;
  }

  //------------------------------------------------------------------------
  bool xpath_object::empty() const
  {
    if (_impl)
      {
        return _impl->empty();
      }
      
    return true;
  }

  //------------------------------------------------------------------------
  const string& xpath_object::expression() const
  {
    if (_impl)
      {
        return _impl->expression();
      }
      
    return simplxpath::empty;
  }

  //------------------------------------------------------------------------
  simplxpath::element xpath_object::operator[] (size_t number)
  {
    try
      {
        if (_impl)
          {
            return simplxpath::element ((*_impl)[number]);
          }
      }
    catch (out_of_range&)
      {
        cerr << "Error: out of range (number: " << number << ")" << endl;
      }

    return simplxpath::element (nullptr);
  }

  //------------------------------------------------------------------------
  const simplxpath::element xpath_object::operator[] (size_t number) const
  {
    try
      {
        if (_impl)
          {
            return simplxpath::element((*_impl)[number]);
          }
      }
    catch (out_of_range&)
      {
        cerr << "Error: out of range (number: " << number << ")" << endl;
      }

    return simplxpath::element (nullptr);
  }

  //------------------------------------------------------------------------
  // element
  //------------------------------------------------------------------------

  //------------------------------------------------------------------------
  element::element (const element& from)
  : _impl (from._impl)
  {
  }

  //------------------------------------------------------------------------
  element::element (const element_impl_ptr& ptr)
  : _impl (ptr)
  {
  }

  //------------------------------------------------------------------------
  element::~element()
  {
  }

  //------------------------------------------------------------------------
  const element& element::operator=  (const element& from)
  {
    if (this != &from)
      {
        _impl = from ._impl;
      }
    return *this;
  }

  //------------------------------------------------------------------------
  const string& element::name() const
  {
    if (_impl)
      {
        return _impl->name();
      }
      
    return empty;
  }

  //------------------------------------------------------------------------
  const string& element::content() const
  {
    if (_impl)
      {
        return _impl->content();
      }
      
    return simplxpath::empty;
  }

  //------------------------------------------------------------------------
  void element::content (const string& text)
  {
    if (_impl)
      {
        _impl->content (text);
      }
  }

  //------------------------------------------------------------------------
  simplxpath::attribute element::attribute (const string& name)
  {
    if (_impl)
      {
        return simplxpath::attribute (_impl->attribute (name));
      }
      
    return simplxpath::attribute (nullptr);
  }

  //------------------------------------------------------------------------
  const simplxpath::attribute element::attribute (const string& name) const
  {
    if (_impl)
      {
        return simplxpath::attribute (_impl->attribute (name));
      }
      
    return simplxpath::attribute (nullptr);
  }

  //------------------------------------------------------------------------
  simplxpath::element element::new_element (const string& name,
                                            const string& content)
  {
    if (_impl)
      {
        return simplxpath::element (_impl->new_element (name, content));
      }
      
    return simplxpath::element (nullptr);
  }

  //------------------------------------------------------------------------
  simplxpath::attribute element::new_attribute (const string& name,
                                                const string& value)
  {
    if (_impl)
      {
        return simplxpath::attribute (_impl->new_attribute (name, value));
      }
      
    return simplxpath::attribute (nullptr);
  }

  //------------------------------------------------------------------------
  // attribute
  //------------------------------------------------------------------------

  //------------------------------------------------------------------------
  attribute::attribute (const attribute& from)
  : _impl (from._impl)
  {
  }

  //------------------------------------------------------------------------
  attribute::attribute (const attribute_impl_ptr& ptr)
  : _impl (ptr)
  {
  }

  //------------------------------------------------------------------------
  attribute::~attribute()
  {
  }

  //------------------------------------------------------------------------
  const attribute& attribute::operator= (const attribute& from)
  {
    if (this != &from)
      {
        _impl = from._impl;
      }
      
    return *this;
  }

  //------------------------------------------------------------------------
  const string& attribute::name() const
  {
    if (_impl)
      {
        return _impl->name();
      }
      
    return simplxpath::empty;
  }

  //------------------------------------------------------------------------
  const string& attribute::value() const
  {
    if (_impl)
      {
          return _impl->value();
      }
      
    return simplxpath::empty;
  }

  //------------------------------------------------------------------------
  void attribute::value( const string& text )
  {
    if (_impl)
      {
        _impl->value (text);
      }
  }

} // simplxpath

