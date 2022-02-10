# cf_yaml
CPDE yml library based on https://github.com/jbeder/yaml-cpp

## Introduction
cf_yaml introduces extension to yaml-cpp to simplyfy yml-parsinf

## Example

```cpp
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

#include "cf_yaml/cf_yaml_doc.h"

const std::string ymltext =
R"YAML(
apiVersion: v3
kind: Pod
metadata:
  name: rss-site
  labels:
    app: web
spec:
  containers:
    - name: front-end
      image: nginx
      ports:
        - containerPort: 80
    - name: rss-reader
      image: nickchase/rss-php-nginx:v1
      ports:
        - containerPort: 88
)YAML";


int main(int argc, char **argv)
{
   try {
      // parse YML text
      std::istringstream in(ymltext);
      if(cf_yaml_doc_ptr doc = cf_yaml_doc::parse(in)) {

         // navigate in the tree and produce some debug print
         cf_yaml_value_ptr root = doc->get_root();

         std::cout << std::endl << "===== debug_print: root " << std::endl;
         root->debug_print(0,cout);
         std::cout << std::endl;

         std::cout << std::endl << "===== debug_print: root->get(\"spec\")->get(\"containers\")->get(1);" << std::endl;

         // move to the "spec" section of the roo matrix
         if( auto spec = root->get("spec",false)) {

            // move to "containers" and get the second (index=1) entry in the squence
            auto matrix = spec->get("containers")->get(1);

            // debug print the matrix there
            matrix->debug_print(9,cout);
            std::cout << std::endl;
         }

      }
   }
   catch(std::exception& ex) {
      std::cout << ex.what() << std::endl;
   }

   return 0;
}
```

```text
<m4> {
   apiVersion:  v3
   kind:  Pod
   metadata:
      <m2> {
         name:  rss-site
         labels:
            <m1> {
               app:  web
             }
       }
   spec:
      <m1> {
         containers:
            <s2> [
               <m3> {
                  name:  front-end
                  image:  nginx
                  ports:
                     <s1> [
                        <m1> {
                           containerPort:  80
                         }
                      ]
                }
               <m3> {
                  name:  rss-reader
                  image:  nickchase/rss-php-nginx:v1
                  ports:
                     <s1> [
                        <m1> {
                           containerPort:  88
                         }
                      ]
                }
             ]
       }
 }

===== debug_print: root->get("spec")->get("containers")->get(1);

         <m3> {
            name:  rss-reader
            image:  nickchase/rss-php-nginx:v1
            ports:
               <s1> [
                  <m1> {
                     containerPort:  88
                   }
                ]
          }
```
