# Threads & Fibers Manager

**Threads & Fibers Manager C++ 20**

I'm using this is for threads & fibers management in game modding, but I guess it could be useful in any C++ projects.
(Fibers are less common though, but the threads manager can be cool !)

# How to use ?

- Creating a thread or a fiber (Can be placed directly into your main.cpp or in an header file...)

```cpp
class yourthread : public thread
{
    public:
        void start() override
        {
        
        }
        
        void update() override
        {
        
        }
        
        void stop() override
        {
        
        }
};

// or...

class yourfiber : public fiber
{
    public:
        void start(bool& _allow_update) override
        {
            _allow_update = true;
        }
        
        void update() override
        {
        
        }
};
```

- This go in your 'main()' function:

```cpp
yourthread thread;
threads_pool::get()->emplace("your_thread_name", &thread);

// or...

yourfiber fiber;
fibers_pool::get()->emplace("your_fiber_name", &fiber);
```

- Removing a thread/fiber by name

```cpp
threads_pool::get()->erase("your_thread_name");

// or...

fibers_pool::get()->erase("your_fiber_name");
```

- Removing all threads/fibers

```cpp
threads_pool::get()->clear();

// or...

fibers_pool::get()->clear();
```
