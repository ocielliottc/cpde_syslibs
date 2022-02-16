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

         std::cout << std::endl << "===== Echo of parsed doc" << std::endl << std::endl;
         cf_yaml_ostream out(cout);
         out << doc;
         std::cout << std::endl;

         std::cout << std::endl;
         std::cout << "===== navigate in the parsed document and output the relevant part"   <<  std::endl;
         std::cout << "===== out << root->get(\"spec\")->get(\"containers\")->get(1);" << std::endl;
         out << doc->get_root()->get("spec")->get("containers")->get(1);
         std::cout << std::endl;

      }
   }
   catch(std::exception& ex) {
      std::cout << ex.what() << std::endl;
   }

   return 0;
}
```

```text
===== Echo of parsed doc

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

===== navigate in the parsed document and output the relevant part
===== out << root->get("spec")->get("containers")->get(1);

---
name: rss-reader
image: nickchase/rss-php-nginx:v1
ports:
  - containerPort: 88
```
