i'm going to be jotting down the development process for this -- hopefully will be fun to look back on and reminisce on my suffering

start: June 3, 00:57:30:
    - end of the first day. today was mostly just figuring out how HNSW works and it's implementation details. one of my professors told me the best (read: only) way to learn how an algorithm works is to implement it. so here i am, implementing this (quite complex) algorithm in a language that i have never touched before. hey, i've always wanted to learn c++ -- what better way to do so than through this? anyways, i didn't really write any real functional code today; it was mostly just getting the boilerplate code up and running (for the vector and HNSW classes, among some other things). my goal is to implement this using only the STL and Eigen library, and hopefully add CUDA support later down the line. but we shall see. 

    - what did i bust my head over?
        - the entire STL library (geeksforgeeks and cppreference my savior)
        - what the heck is a template (i now know)
        - some very strange design choices in the STL (especially for object hashing), although they may only be strange to my unenlightened, feeble mind
        - just how well my C knowledge is transferring over (and how scary this is because everyone says the biggest mistake you can make w C++ is treating it like C w classes...)
        - ***the actual HNSW algorithm itself***
            - i probably have to reread the paper from top-bottom again... ah well. 

June 4, 01:26:47:
    - end of the second day. building off of yesterday, i finished understanding the HNSW algorithm earlier today. my first implementation was quite bad (read: nonfunctional), as i was also figuring out C++ reference + pointer semantics lol (shared_ptr who?). anyways, i now have the insert functionality completely working (albeit quite slow -- this will probably not change until i parallelize). all that is left to do is implement the search.
    
    - what did i bust my head over today?
        - the STL library again (specifically how C++ deals with pointers! it's very confusing coming from someone who is v familiar w C but very rusty when it comes to C++). i also had some issues figuring out how std::make_heap and the other STL heap functions work -- i ended up implementing my own Heap class with a std::vector + std::unordered_set backing store. 
        - actually implementing the insert algorithm
            - pretty much everything that could have gone wrong went wrong, but then (as things usually do), a few fixes here and there magically got everything working. some of my biggest problems were 
                - figuring out when/where i need to use maxheap() / minheap() (it turns out that the per-level search is basically just an A* search + a maxheap to track candidates; if only i figured that out earlier...)
                - getting a bit confused over distance vs similarity metrics (ended up getting some quite terrible performance early on; it turned out i was using the raw similarity as a means to calculate the distance, giving me the WORSE candidates!)
                - basic code structure; what goes where and how?
                - lots more!
    but i figured it out. now, all that is left to do is implement the search and (hopefully) this should be a somewhat accurate implementation. stay tuned!
