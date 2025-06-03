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
