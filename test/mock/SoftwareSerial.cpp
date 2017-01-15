#include "SoftwareSerial.h"

#include <iostream>
using namespace std;

SoftwareSerial::SoftwareSerial(int pinRx, int pinTx){
}
void SoftwareSerial::begin(int rate){
}
void SoftwareSerial::listen(){
}
char SoftwareSerial::read(){
  return '\0';
}
int SoftwareSerial::available(){
  return 0;
}
void SoftwareSerial::print( const char *msg){
//  cout << msg;
}
void SoftwareSerial::print( const __FlashStringHelper *msg){
//  cout << msg;
}
void SoftwareSerial::print( const char c){
//  cout << c;
}
void SoftwareSerial::print( int msg){
//  cout << msg;
}
void SoftwareSerial::print( long msg){
//  cout << msg;
}
void SoftwareSerial::println( const char *msg){
//  cout << msg;
}
void SoftwareSerial::println( const __FlashStringHelper *msg){
//  cout << msg;
}
void SoftwareSerial::println( const char c){
//  cout << c;
}
void SoftwareSerial::println( int msg){
//  cout << msg;
}
void SoftwareSerial::println( long msg){
//  cout << msg;
}
