#include <simplxpath.hpp>
#include <iostream>

using namespace std;
using namespace simplxpath;

int main( int argc, const char** argv )
{
  cout << "START XMLTEST" << endl;

  {
    xmldoc doc ("xmltest.xml");

    if (! doc.valid())
      {
        /* dumpsize = 0 -> for xml content be used full dump str */
        xmldoc doc( "<root></root>", 0 );
        cout << "1:" << doc.filename() << endl;

        /* if filename is not defined (either save() or constructor), */
        /* then save to default.xml                                   */
        doc.save();
        cout << "2:" << doc.filename() << endl;

        doc.save( "xmltest.xml" );
        cout << "3:" << doc.filename() << endl;
      }
  }

  {
    xmldoc doc ("xmltest.xml");
  
    if (doc.valid())
      {
        cout << doc.filename() << endl;

        xpath_object object = doc["/ROOT"];
        if( object.empty() )
        {
          cerr << "not found objects " << object.expression() << endl;
        }

        object = doc["/root"];
        if (object.size())
        {
            cout << "found [" << object.size() << "] objects for " << object.expression() << endl;

            element elm = object[999];
            /* if number is "out_of_range" then */
            /* it will return empty element     */
            if( elm.name().empty() )
            {
                cout << "it is empty element" << endl;
                cout << "elm name:    " << elm.name()    << endl;
                cout << "elm content: " << elm.content() << endl;

                attribute atr = elm.new_attribute ("branchs", "2"); // do nothing because
                element   br1 = elm.new_element   ("branch");       // elm
                element   br2 = elm.new_element   ("branch");       // is not exist

                cout << "br1 name:    " << br1.name()    << endl;
                cout << "br1 content: " << br1.content() << endl;
                cout << "br2 name:    " << br2.name()    << endl;
                cout << "br2 content: " << br2.content() << endl;
                cout << "atr name:    " << atr.name()    << endl;
                cout << "atr value:   " << atr.value()   << endl;

                element   br3 = br1.new_element( "leaf" ); // do nothing because ...
                cout << "br3 name:    " << br3.name()    << endl;
                cout << "br3 content: " << br3.content() << endl;

                atr = br3.new_attribute( "n", "0" );       // do nothing because ... 
                cout << "atr name:    " << atr.name()    << endl;
                cout << "atr value:   " << atr.value()   << endl;
            }

            elm = object[0]; // get root
            if( elm.name().size() )
            {
                cout << "it is correct element"            << endl;
                cout << "elm name:      " << elm.name()    << endl;
                cout << "elm content:   " << elm.content() << endl;
                cout << "add new elements" << endl;

                attribute atr = elm.new_attribute( "branchs", "2" );
                element   br1 = elm.new_element( "branch" );
                element   br2 = elm.new_element( "branch" );

                cout << "br1 name:    " << br1.name()    << endl;
                cout << "br1 content: " << br1.content() << endl;
                cout << "br2 name:    " << br2.name()    << endl;
                cout << "br2 content: " << br2.content() << endl;
                cout << "atr name:    " << atr.name()    << endl;
                cout << "atr value:   " << atr.value()   << endl;

                element   lf = br1.new_element( "leaf" );

                cout << "br3 name:    " << lf.name()    << endl;
                cout << "br3 content: " << lf.content() << endl;

                atr = lf.new_attribute( "n", "3" );     
                cout << "atr name:    " << atr.name()  << endl;
                cout << "atr value:   " << atr.value() << endl;

                atr.value( "0" ); // change value
                cout << "atr value:   " << atr.value() << endl;                

                doc.save();
            }
          }
        }
  }
    
  cout << "END XMLTEST" << endl;
  return 0;
}
