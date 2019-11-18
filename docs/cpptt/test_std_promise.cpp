//
// Created by zhangbin on 2019-11-08.
//
/*
 * With std::promise and std::future, you have the full control over the task.

Full control over the task
A std::promise permits

to set a value, a notification or an exception.
 That result can in addition delayed be provided by the promise.
A std::future permits to

pick up the value from the promise.
asks the promise, if the value is available.
wait for the notification of the promise.
 That waiting can be done with a relative time duration or an absolute time point.
 => Replacement for condition variables.
create a shared future (std::shared_future).
 * */
#include<future>
#include<iostream>
#include<thread>
#include<utility>

void product(std::promise<int> && intPromise,int a,int b){

intPromise.set_value(a*b);
}
struct Div {
  void operator() (std::promise<int> && intPromise,int a, int b ) const {
    intPromise.set_value(a/b);
  }
};
int main()
{
  int a=20;
  int b =10;
  std::cout <<std::endl;
  //define the promises
  std::promise<int> prodPromise;
  std::promise<int> divPromise;

  //get the futures
  std::future<int> proResult = prodPromise.get_future();
  std::future<int> divResult = divPromise.get_future();

  //product needs a promise of the kind rvalue reference
  // (std::promise<int>&& intPromise) and two numbers.
  // These are exactly the last three arguments of the thread prodThread.
  // std::move in line 36 creates the rvalue reference.
  //caculate the result in a seperate thread
  std::thread prodThread(product,std::move(prodPromise),a,b);

  //这个用法比较少见啊
  //For is job, it uses the instances div of the class Div (line 12 - 18). div is a function object.
  Div div;
  std::thread divThread(product,std::move(divPromise),a,b);

  //get the result
  std::cout << "20*20= "<<proResult.get() <<std::endl;
  std::cout<<" 20/10 = "<< divResult.get() <<std::endl;

  prodThread.join();
  divThread.join();
  std::cout <<std::endl;

}