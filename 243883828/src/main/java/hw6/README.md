# Discussion

## Unit testing TreapMap
It was difficult to adapt to the set numbers that were outputted by the 
random seeds. Firstly, I had to figure out what numbers were being outputted
and in what order from the seed I was using. I did this by inserting values and running
it to see what numbers for the priority comes out. Secondly, it was
difficult to only be able to make tests for trees in that order. When I 
wanted a smaller priority to come first before a higher priority or vice versa,
I could not change the order and had to work around it. I found the easiest
ways to make a test was to make the general shape of the set-up for the 
rotation I wanted to make, then filling in the priorities in the tree that
would cause the rotation, and lastly matching numerical values that would satisfy the 
binary tree property. 

Here is an example from the insertLeftRightRotation test:

Since I wanted a left-right rotation I knew I needed this shape somewhere in the tree:
    ___
    /
 ___
   \
    ___

However, the values from my seed came out in this order: (the numbers in the parentheses correlate
to its ranking from greatest to least priority in a min tree)

-1157793070 (2)
1913984760(7)
1107254586(5)
1773446580(6)
254270492(3)
-1408064384(1)
1048475594(4)

I needed two values where the priorities are in order until a third priority that is 
greater than both of them is inserted. The first sequence of numbers with that
requirement is 1107254586(5), 1773446580(6), 254270492(3). So to bypass the first 
two numbers, I need to make the left-right rotation a subtree of the first number,
and make the second number another subtree so that it is out of the way.

(Numbers represent the priorities)

    2 <-- not part of the rotation
   /   \
  5     7  <-- not part of the rotation
 /
6
 \
  3 <-- inserted

Then I match it with numbers that would satisfy a binary tree:

    4(2)
    /   \
   3(5)  5(7)  
   /
 1(6)
    \
    2(3) <-- inserted

This should make a left-right rotation as 3 is smaller than 6, so it does a left
rotation, then 3 is still smaller than 5, so it makes a right rotation and ends in:

     4(2)
    /   \
  2(3)  5(7)
 /    \
1(6)  3(5)

Now both the binary properties and the priority order is satisfied.

It was more challenging to think of a test for removal as I needed to
find an initial tree that already satisfies the treap properties and then
must do a specific rotation to remove.

Here is an example of how I made a removal test with the removeLeftRotationOneChild() test:

I know that I need this shape to make a left rotation:
___
  \
  ___
    \
    ___

So I need consecutive values where the first has more priority than the next.
The challenge here is that the seed outputs the worst priority second.
So, to divert this, I put it in a separate tree and then remove it after, which
should not cause any rotation: (parentheses is its priority ranking)
        
                            2(2) <-- priority changes to MAX for deletion
                            /  \
removed (no rotation)--> 1(7) 3(5)
                                \
                                4(6)

Then when removing 2, the priority changes to MAX so the tree has to rotate left to
readjust the priority order.

        3(5) 
        /  \
    2(MAX) 4(6)

And now since 2 is a leaf, it is simple to remove from the tree and there should
be no further rotations.
        3(5) 
           \
           4(6)

Another challenge was removing an element that has two children because this 
would require two rotations where initially the root has higher priority than
its children. In this case, my seed works in my favor as the priority ranking 
for the first three outputs are 2, 7, and 5. Here is an example of my 
removeLeftRightRotationTwoChildren() test.

It initially starts with this 

        2(2)  <-- priority changes to MAX for deletion
        /  \
    1(7)   3(5)

Then, when the priority of 2 changes to MAX, the treap needs to fix the priority 
ordering by rotating with the child with a greater priority. Therefore, it should
rotate with 3 in a left rotation.

        3(5)  
        /  
    2(MAX)
      /
    1(7)

Now the 2 and 3 are ordered in priority but 2 and 1 are out of order. To fix this,
2 and 1 need to preform a right rotation.

        3(5)  
        /  
      1(7)
        \
       2(MAX)

Now that everything is ordered properly, 2 is now a leaf and can be easily removed.
No more rotations should occur after.
    3(5)  
    /  
  1(7)



## Benchmarking
Hotel California:
Benchmark                  Mode  Cnt  Score   Error  Units
JmhRuntimeTest.arrayMap    avgt    2  0.508          ms/op
JmhRuntimeTest.avlTreeMap  avgt    2  0.382          ms/op
JmhRuntimeTest.bstMap      avgt    2  0.374          ms/op
JmhRuntimeTest.treapMap    avgt    2  0.700          ms/op


Moby Dick:
Benchmark                  Mode  Cnt     Score   Error  Units
JmhRuntimeTest.arrayMap    avgt    2  9074.696          ms/op
JmhRuntimeTest.avlTreeMap  avgt    2   409.495          ms/op
JmhRuntimeTest.bstMap      avgt    2   365.178          ms/op
JmhRuntimeTest.treapMap    avgt    2   467.346          ms/op

Federalist
Benchmark                  Mode  Cnt  Score   Error  Units
JmhRuntimeTest.arrayMap    avgt    2  8.873          ms/op
JmhRuntimeTest.avlTreeMap  avgt    2  3.201          ms/op
JmhRuntimeTest.bstMap      avgt    2  3.237          ms/op
JmhRuntimeTest.treapMap    avgt    2  3.272          ms/op

Pride and Prejudice:
Benchmark                  Mode  Cnt     Score   Error  Units
JmhRuntimeTest.arrayMap    avgt    2  1707.784          ms/op
JmhRuntimeTest.avlTreeMap  avgt    2   160.344          ms/op
JmhRuntimeTest.bstMap      avgt    2   166.913          ms/op
JmhRuntimeTest.treapMap    avgt    2   136.761          ms/op

Conclusions:
The benchmark tests show that the array map usually takes the longest 
while the other three are usually around the same runtime. This is most
evident in the federalist paper as it takes 8.873 ms/op for the array map
while it takes ~3 ms/op for every other map. This is because the insertion 
operation always needs to implement a search for the word it wants to input
to make sure there are no duplicates. In doing this, an array will always have
to linearly search for the word while the others, on average, can search for
the word in O(log n) time. 

As the source document becomes longer, like Moby Dick and Pride and Prejudice, 
the array map runtime is still a lot larger than the others, but the others 
start to have a slight difference. In Pride and Prejudice, they are still mostly 
the same with the AVL tree map and the BST map being both around 160 ms/op, but
there is a ~30 ms/op difference with the treap map. This could be due to the random
priority values that the Treap assigns to each word making the tree more efficient
than the others. In Moby Dick however, the BST is the more efficient than the other two. This may be
because the text is random enough, so it avoids the worst case where the BST becomes 
linear and would end up having to linearly search for the word. Because of this,
the AVL tree and the Treap end up doing extra work to keep the balance/priority order.

As the source document becomes smaller, however, seen in the Hotel California text, we
experience the same trend where the array map is slower than the rest while the rest 
are normally around the same run time. There is a discrepancy with the Treap map here, 
however, where it is a lot longer than the rest. Again, this may be due to its random
assignment of priority for the words that make it less efficient than the rest. 
