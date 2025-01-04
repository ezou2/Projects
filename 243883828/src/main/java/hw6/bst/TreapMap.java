package hw6.bst;

import static java.lang.Integer.MAX_VALUE;

import hw6.OrderedMap;
import java.util.Iterator;
import java.util.Random;
import java.util.Stack;

/**
 * Map implemented as a Treap.
 *
 * @param <K> Type for keys.
 * @param <V> Type for values.
 */
public class TreapMap<K extends Comparable<K>, V> implements OrderedMap<K, V> {

  /*** Do not change variable name of 'rand'. ***/
  private static Random rand;
  /*** Do not change variable name of 'root'. ***/
  private Node<K, V> root;

  private int size;

  /**
   * Make a TreapMap.
   */
  public TreapMap() {
    rand = new Random();
  }

  /**
   * Map implemented as a Treap with a seed for its random value. For testing purposes.
   * @param seed the seed for the Random() function.
   */
  public TreapMap(int seed) {
    rand = new Random(seed);
  }

  //Right rotation
  private TreapMap.Node<K, V> rightRotation(TreapMap.Node<K, V> node) {
    TreapMap.Node<K, V> child = node.left;
    node.left = child.right;
    child.right = node;
    node = child;
    return node;
  }

  //left rotation
  private TreapMap.Node<K, V> leftRotation(TreapMap.Node<K, V> node) {
    TreapMap.Node<K, V> child = node.right;
    node.right = child.left;
    child.left = node;
    node = child;
    return node;
  }

  //return true when the element has two children
  private boolean twoChildren(TreapMap.Node<K,V> parent) {
    return parent.right != null && parent.left != null;
  }

  private TreapMap.Node<K, V> updateNode(TreapMap.Node<K, V> node) {
    // if the node has two children in which both have a greater priority (smaller number) than the parent node
    if (twoChildren(node) && ((node.priority > node.right.priority) && (node.priority > node.left.priority))) {
      // rotate with the child that has more priority
      if (node.right.priority < node.left.priority) { // right has more priority
        node = leftRotation(node);
      } else { //left has more priority
        node = rightRotation(node);
      }
      return updateNode(node); // recurse to ensure the tree is fully updated
    } else if (node.right != null && node.right.priority < node.priority) {
      // do a left rotation if right child exists and has a higher priority
      node = leftRotation(node);
      return updateNode(node); // recurse to ensure the tree is fully updated
    } else if (node.left != null && node.left.priority < node.priority) {
      // do a right rotation if left child exists and has a higher priority
      node = rightRotation(node);
      return updateNode(node); // recurse to ensure the tree is fully updated
    } else {
      return node; // return final updated node
    }
  }

  private TreapMap.Node<K, V> insert(TreapMap.Node<K, V> n, K k, V v) {
    // found place to insert node
    if (n == null) {
      return new TreapMap.Node<>(k, v);
    }

    // finding place to insert node
    int cmp = k.compareTo(n.key);
    if (cmp < 0) {
      n.left = insert(n.left, k, v);
    } else if (cmp > 0) {
      n.right = insert(n.right, k, v);
    } else {
      throw new IllegalArgumentException("duplicate key " + k);
    }

    n = updateNode(n); // fix priority order if needed

    return n;
  }

  @Override
  public void insert(K k, V v) throws IllegalArgumentException {
    if (k == null) {
      throw new IllegalArgumentException("cannot handle null key");
    }
    root = insert(root, k, v);
    size++;
  }

  // Return a node with maximum key in subtree rooted at given node.
  private TreapMap.Node<K, V> max(TreapMap.Node<K, V> node) {
    TreapMap.Node<K, V> curr = node.left;
    while (curr.right != null) {
      curr = curr.right;
    }
    return curr;
  }

  // sink node to remove down to a leaf
  private TreapMap.Node<K, V> sinkNode(TreapMap.Node<K, V> node) {
    // if the node no longer has children, it is a leaf and ready to remove
    if (node.right == null && node.left == null) {
      return remove(node);
    } else { // not a leaf
      TreapMap.Node<K, V> newNode = updateNode(node); // moves the node down one level
      if (newNode.right != null && newNode.right.key.compareTo(node.key) == 0) {
        // if the node to remove ended up on the right, keep sinking the right node
        node = newNode.right;
        newNode.right = sinkNode(node);
      } else if (newNode.left != null && newNode.left.key.compareTo(node.key) == 0) {
        // if the node to remove ended up on the left, keep sinking the left node
        node = newNode.left;
        newNode.left = sinkNode(node);
      }
      return newNode;
    }
  }

  private TreapMap.Node<K, V> remove(TreapMap.Node<K, V> subtreeRoot, TreapMap.Node<K, V> toRemove) {
    int cmp = subtreeRoot.key.compareTo(toRemove.key);
    if (cmp == 0) {
      return sinkNode(subtreeRoot);
    } else if (cmp > 0) {
      subtreeRoot.left = remove(subtreeRoot.left, toRemove);
    } else {
      subtreeRoot.right = remove(subtreeRoot.right, toRemove);
    }
    return subtreeRoot;
  }

  private TreapMap.Node<K, V> remove(TreapMap.Node<K, V> node) {
    // remove only when node has 0 children
    if (node.right == null) {
      return node.left;
    } else if (node.left == null) {
      return node.right;
    }

    return node;
  }

  @Override
  public V remove(K k) throws IllegalArgumentException {
    // TODO Implement Me!
    TreapMap.Node<K, V> node = findForSure(k);
    node.priority = MAX_VALUE; // change the node to the least priority
    V value = node.value;
    root = remove(root, node);
    size--;
    return value;
  }

  private TreapMap.Node<K, V> findForSure(K k) {
    TreapMap.Node<K, V> n = find(k);
    if (n == null) {
      throw new IllegalArgumentException("cannot find key " + k);
    }
    return n;
  }

  private TreapMap.Node<K, V> find(K k) {
    if (k == null) {
      throw new IllegalArgumentException("cannot handle null key");
    }
    TreapMap.Node<K, V> n = root;
    while (n != null) {
      int cmp = k.compareTo(n.key);
      if (cmp < 0) {
        n = n.left;
      } else if (cmp > 0) {
        n = n.right;
      } else {
        return n;
      }
    }
    return null;
  }

  @Override
  public void put(K k, V v) throws IllegalArgumentException {
    // TODO Implement Me!
    TreapMap.Node<K, V> n = findForSure(k);
    n.value = v;
  }

  @Override
  public V get(K k) throws IllegalArgumentException {
    // TODO Implement Me!
    TreapMap.Node<K, V> n = findForSure(k);
    return n.value;
  }

  @Override
  public boolean has(K k) {
    // TODO Implement Me!
    if (k == null) {
      return false;
    }
    return find(k) != null;
  }

  @Override
  public int size() {
    // TODO Implement Me!
    return size;
  }

  @Override
  public Iterator<K> iterator() {
    // TODO Implement Me!
    return new InorderIterator();
  }

  /*** Do not change this function's name or modify its code. ***/
  @Override
  public String toString() {
    return BinaryTreePrinter.printBinaryTree(root);
  }


  /**
   * Feel free to add whatever you want to the Node class (e.g. new fields).
   * Just avoid changing any existing names, deleting any existing variables,
   * or modifying the overriding methods.
   * Inner node class, each holds a key (which is what we sort the
   * BST by) as well as a value. We don't need a parent pointer as
   * long as we use recursive insert/remove helpers. Since this is
   * a node class for a Treap we also include a priority field.
   **/
  private static class Node<K, V> implements BinaryTreeNode {
    Node<K, V> left;
    Node<K, V> right;
    K key;
    V value;
    int priority;

    // Constructor to make node creation easier to read.
    Node(K k, V v) {
      // left and right default to null
      key = k;
      value = v;
      priority = generateRandomInteger();
    }

    // Use this function to generate random values
    // to use as node priorities as you insert new
    // nodes into your TreapMap.
    private int generateRandomInteger() {
      // Note: do not change this function!
      return rand.nextInt();
    }

    @Override
    public String toString() {
      return key + ":" + value + ":" + priority;
    }

    @Override
    public BinaryTreeNode getLeftChild() {
      return left;
    }

    @Override
    public BinaryTreeNode getRightChild() {
      return right;
    }
  }

  private class InorderIterator implements Iterator<K> {
    private final Stack<TreapMap.Node<K, V>> stack;

    InorderIterator() {
      stack = new Stack<>();
      pushLeft(root);
    }

    private void pushLeft(TreapMap.Node<K, V> curr) {
      while (curr != null) {
        stack.push(curr);
        curr = curr.left;
      }
    }

    @Override
    public boolean hasNext() {
      return !stack.isEmpty();
    }

    @Override
    public K next() {
      TreapMap.Node<K, V> top = stack.pop();
      pushLeft(top.right);
      return top.key;
    }
  }
}
