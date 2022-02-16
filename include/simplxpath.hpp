#ifndef __SIMPLXPATH_HPP__
#define __SIMPLXPATH_HPP__

  #ifdef SIMPLXPATH_EXPORT_DLL

    #define SIMPLXPATH_DECL __declspec(dllexport)

  #elif defined (SIMPLXPATH_IMPORT_DLL)

    #define SIMPLXPATH_DECL __declspec(dllimport)

  #else

    #define SIMPLXPATH_DECL

  #endif

#include <memory>
#include <string>

namespace simplxpath
{
  class xmldoc;
  class xpath_object;
  class element;
  class attribute;

  class xmldoc_impl;
  class xpath_object_impl;
  class element_impl;
  class attribute_impl;

  typedef std::shared_ptr< xmldoc_impl       > xmldoc_impl_ptr;
  typedef std::shared_ptr< xpath_object_impl > xpath_object_impl_ptr;
  typedef std::shared_ptr< element_impl      > element_impl_ptr;
  typedef std::shared_ptr< attribute_impl    > attribute_impl_ptr;

  //------------------------------------------------------------------------
  class SIMPLXPATH_DECL xmldoc
  {
  public:
             xmldoc (const xmldoc&);
    explicit xmldoc (const std::string& filename);
             xmldoc (const std::string& dump, size_t dumpsize);
            ~xmldoc ();

    xmldoc&            operator=  (const xmldoc&);
    bool               valid      () const;
    const std::string& filename   () const;
    xpath_object       operator[] (const std::string& expression);
    void               save       (const std::string& filename = "");

  private:
    xmldoc_impl_ptr _impl;
  };
    
  //------------------------------------------------------------------------
  class SIMPLXPATH_DECL xpath_object
  {
  public:
             xpath_object (const xpath_object&);
    explicit xpath_object (const xpath_object_impl_ptr&);
            ~xpath_object ();

    const xpath_object& operator=  (const xpath_object&);
          size_t        size       () const;
          bool          empty      () const;    
    const std::string&  expression () const;
          element       operator[] (size_t number);
    const element       operator[] (size_t number) const;
              
  private:
    xpath_object_impl_ptr _impl;
  };
    
  //------------------------------------------------------------------------
  class SIMPLXPATH_DECL element
  {
  public:
             element (const element&);
    explicit element (const element_impl_ptr&);
            ~element ();

    const
      simplxpath::element& operator= (const simplxpath::element&);
    const
      std::string&    name      () const;
    const
      std::string&    content   () const;
      void            content   (const std::string& text);
      simplxpath::attribute attribute (const std::string& name);
    const
      simplxpath::attribute attribute     (const std::string& name) const;
      simplxpath::element   new_element   (const std::string& name,
                                           const std::string& content = "" );
      simplxpath::attribute new_attribute (const std::string& name,
                                           const std::string& value   = "" );
  private:
    element_impl_ptr _impl;

  };
    
  //------------------------------------------------------------------------
  class SIMPLXPATH_DECL attribute
  {
  public:
             attribute (const attribute&);
    explicit attribute (const attribute_impl_ptr&);
            ~attribute ();
        
    const attribute&   operator= (const attribute&);
    const std::string& name  () const;
    const std::string& value () const;
          void         value (const std::string& text);
        
  private:
    attribute_impl_ptr _impl;
  };
  
} // simplxpath

#endif // __SIMPLXPATH_HPP__

