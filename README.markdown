Thread Pool
=============

A simple thread pool implementation.

Instalation
-----------

Add the source folder to your compilation path.

Usage
-----

Submit to the queue the functions you want to run in parallel, example:

    {
      int aux1, aux2;
      auto f1 = []() { aux1 = 2; };
      auto f2 = []() { aux2 = 4; };

      ThreadPool<std::function<void()>> thread_pool;
      thread_pool.submit(f1);
      thread_pool.submit(f2);
    } // destructor joins threads

    assert(aux1 == 2);
    assert(aux2 == 4);


License
-------

LGPL