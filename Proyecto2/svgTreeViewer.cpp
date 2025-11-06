#include <iostream>
#include <sstream>
#include "bstTree.cpp"

using namespace std;

int main(){

  ArbolSVG aInt = ArbolSVG(T(int));
  int ints[] = {20, 30, 2, 56, 768 };
  aInt.insert(&ints[0]); //      20
  aInt.insert(&ints[1]); //  2       30
  aInt.insert(&ints[2]); //              56
  aInt.insert(&ints[3]); //                  768
  aInt.insert(&ints[4]);
  cout<<"Hello World:"<<endl;
  cout<<aInt.toString()<<endl;
  aInt.toSVG("aInt_svgTreeViewer.html");
  
  // niveles: 20, 2, 30, 56, 768
  // preorder: 20, 2, 30, 56, 768
  // inorder: 2, 20, 30, 56, 768
  // postorder: 2, 768, 56, 30, 20
  
  ArbolSVG aStr = ArbolSVG(T(string));
  string strs[] = {"b", "a", "c"};
  aStr.insert(&strs[0]); //      b
  aStr.insert(&strs[1]); //  a       c
  aStr.insert(&strs[2]);
  cout<<"Hello World:"<<endl;
  cout<<aStr.toString()<<endl;
  aStr.toSVG("aStr_svgTreeViewer.html");
  
  // niveles: b, a, c
  // preorder: b, a, c
  // inorder: a, b, c
  // postorder: a, c, b
  
  ArbolSVG aInt2 = ArbolSVG(T(int));
  int ints2[] = {
    50, 30, 70, 20, 40, 60, 80,
    55, 35, 75, 25, 45, 65, 85,
    52, 32, 72, 22, 42, 62, 82,
    58, 38, 78, 28, 48, 68, 88,
    54, 34, 74, 24, 44, 64, 84,
    56, 36, 76, 26, 46, 66, 86,
    53, 33, 73, 23, 43, 63, 83,
    57, 37, 77, 27, 47, 67, 87,
    51, 31, 71, 21, 41, 61, 81,
    59, 39, 79, 29, 49, 69, 89
  };
  
  int ints2Len = sizeof(ints2) / sizeof(ints2[0]);
  
  for(int i = 0; i < ints2Len; i++){
    aInt2.insert(&ints2[i]);
  }
  aInt2.toSVG("aInt2_svgTreeViewer.html");
  
  return 0;
}
