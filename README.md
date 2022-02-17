# libsimplxpath
The simple C++ library for using some of the libxml2 xpath capabilities

Briefly guide
-------------
```cpp
  #include <simplxpath.hpp>
  using namespace libsimplxpath;

// Open a new xml document:

  xmldoc doc ("something.xml");

// You can try to check it for validity:

  if (!doc.valid())
    {
      ...
      // I am so sorry
      ...
    }

// Make an xpath request:
  xpath_object object = doc["/root/branch/leaf[@n=\"0\"]"];

  // if doc is not valid, then object size will be zero
  // 0_o-/ sizeroooooo! :-D

  for (size_t i = 0; i < object.size(); ++i)
    {
      ...
      // something was founded to process
      element el = object[i];
      cout << "element:"  << endl;
      cout << "name:    " << el.name()    << endl;
      cout << "content: " << el.content() << endl;

      attribute attr = el.attribute ("n");
      cout << "attribute:" << endl;
      cout << "name:     " << attr.name()  << endl;
      cout << "content:  " << attr.value() << endl;
      ...
    }

  // You can add new elements to your xml document:
  xmldoc doc ("<root></root>", 0);

  xpath_object obj = doc["/root"];

  if (obj.size())
    {
      element el = obj[0];
      el.new_element   ("branch", "content");
      el.new_attribute ("attr",   "value"  );
    }

  doc.save ("something.xml);
```
