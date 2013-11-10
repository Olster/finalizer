#include <iostream>

#include "finalizer.h"

using namespace std;

// Test single finalizer.
void func() {
  // Let's allocate a few pointers.
  char* a = new char[2000];
  char* b = new char[2000];
  char* c = new char[2000];
  char* d = new char[2000];

  Finalizer finalize([&]() {
    delete [] a;
    delete [] b;
    delete [] c;
    delete [] d;

    cout << "Memory cleared" << endl;
  });

  cout << "Doing something with memory" << endl;
  memset(a, 0, 2000);
  memset(b, 0, 2000);
  memset(c, 0, 2000);
  memset(d, 0, 2000);

  cout << "Returning" << endl;
}

void trace() {
  cout << "Function exited" << endl;
}

void func2() {
  Finalizer fin(trace);

  cout << "Hello fn" << endl;
}

// Multiple finalizers.
void func3() {
  // Please note that order really matters: because these variables are placed
  // on stack, they will be destroyed in a reversed order.
  // So the output is:
  // Finalizer 3 called
  // Finalizer 2 called
  // Finalizer 1 called
  auto fn = [](int id) {
    cout << "Finalizer " << id << " called" << endl;
  };

  Finalizer fin1(fn, 1);
  Finalizer fin2(fn, 2);
  Finalizer fin3(fn, 3);

  // Doing some work here.
}

int main() {
  //func();
  //func2();
  func3();

  cin.ignore();
  return 0;
}