package hw6.bst;

import hw6.OrderedMap;
import java.util.Iterator;
import java.util.Stack;

/**
 * Map implemented as an AVL Tree.
 *
 * @param <K> Type for keys.
 * @param <V> Type for values.
 */
public class AvlTreeMap<K extends Comparable<K>, V> implements OrderedMap<K, V> {

  /*** Do not change variable name of 'root'. ***/
  private Node<K, V> root;

  private int size;

  // right rotation
  private AvlTreeMap.Node<K, V> rightRotation(AvlTreeMap.Node<K, V> node) {
    AvlTreeMap.Node<K, V> child = node.left;
    node.left = child.right;
    child.right = node;
    node = child;
    return node;
  }

  // left rotation
  private AvlTreeMap.Node<K, V> leftRotation(AvlTreeMap.Node<K, V> node) {
    AvlTreeMap.Node<K, V> child = node.right;
    node.right = child.left;
    child.left = node;
    node = child;
    return node;
  }

  // updates the height of nodes
  private int updateHeight(AvlTreeMap.Node<K, V> left, AvlTreeMap.Node<K, V> right) {
    int height = 0;
    // if the node has both children, add 1 to the greater height
    if (left != null && right != null) {
      height = Math.max(left.height, right.height) + 1;
    } else {
      // if it has no children
      if (left == null && right == null) {
        return height; // already 0
      } else if (left != null) { // has a left child, but no right child
        height = left.height + 1;
      } else { // has a right child, but no left child
        height = right.height + 1;
      }
    }
    return height;
  }

  private int updateBalance(AvlTreeMap.Node<K, V> node) {
    int leftHeight;
    int rightHeight;

    // if there is no child node, its height is -1
    if (node.left == null) {
      leftHeight = -1;
    } else {
      leftHeight = node.left.height;
    }

    if (node.right == null) {
      rightHeight = -1;
    } else {
      rightHeight = node.right.height;
    }

    node.balanceFactor = leftHeight - rightHeight;

    return node.balanceFactor;

  }

  // decides which way to rotate
  private AvlTreeMap.Node<K, V> rotate(AvlTreeMap.Node<K, V> node) {
    // if parent is left leaning
    if (node.balanceFactor > 0) {
      //if left child is left leaning or balanced
      if (node.left.balanceFactor >= 0) {
        node = rightRotation(node);

      } else { //left child is right leaning, double left-right rotation
        node.left = leftRotation(node.left);
        node = rightRotation(node);
      }
    } else { //parent is right-leaning
      // right child is right-leaning
      if (node.right.balanceFactor <= 0) {
        node = leftRotation(node);
      } else { // right child is left leaning, double right-left rotation
        node.right = rightRotation(node.right);
        node = leftRotation(node);
      }
    }
    return node;
  }

  // balancing the tree
  private AvlTreeMap.Node<K, V> checkBalance(AvlTreeMap.Node<K, V> node) {
    // node is balanced if its balance factor is 1, 0, or -1
    //if not balanced
    if (!(node.balanceFactor == 1 || node.balanceFactor == 0 || node.balanceFactor == -1)) {
      AvlTreeMap.Node<K, V> oldNode = node;
      node = rotate(node); // needs to rotate

      // balances all children too
      if (node.right != null) {
        node.right = updateNode(node.right);
      }
      if (node.left != null) {
        node.left = updateNode(node.left);
      }
      node = updateNode(node);
    }
    return node;
  }

  //updates node's height and balances it
  private AvlTreeMap.Node<K, V> updateNode(AvlTreeMap.Node<K, V> n) {
    n.height = updateHeight(n.left, n.right);
    n.balanceFactor = updateBalance(n);
    n = checkBalance(n);

    return n;
  }

  private AvlTreeMap.Node<K, V> find(K k) {
    if (k == null) {
      throw new IllegalArgumentException("cannot handle null key");
    }
    AvlTreeMap.Node<K, V> n = root;
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

  private AvlTreeMap.Node<K, V> findForSure(K k) {
    AvlTreeMap.Node<K, V> n = find(k);
    if (n == null) {
      throw new IllegalArgumentException("cannot find key " + k);
    }
    return n;
  }


  // Return a node with maximum key in subtree rooted at given node.
  private AvlTreeMap.Node<K, V> max(AvlTreeMap.Node<K, V> node) {
    AvlTreeMap.Node<K, V> curr = node.left;
    while (curr.right != null) {
      curr = curr.right;
    }
    return curr;
  }

  private AvlTreeMap.Node<K, V> insert(AvlTreeMap.Node<K, V> n, K k, V v) {
    if (n == null) {
      return new AvlTreeMap.Node<>(k, v);
    }

    int cmp = k.compareTo(n.key);
    if (cmp < 0) {
      n.left = insert(n.left, k, v);
    } else if (cmp > 0) {
      n.right = insert(n.right, k, v);
    } else {
      throw new IllegalArgumentException("duplicate key " + k);
    }

    n = updateNode(n);

    return n;
  }

  @Override
  public void insert(K k, V v) throws IllegalArgumentException {
    // TODO Implement Me!
    if (k == null) {
      throw new IllegalArgumentException("cannot handle null key");
    }
    root = insert(root, k, v);
    size++;
  }

  private AvlTreeMap.Node<K, V> remove(AvlTreeMap.Node<K, V> subtreeRoot, AvlTreeMap.Node<K, V> toRemove) {
    int cmp = subtreeRoot.key.compareTo(toRemove.key);
    if (cmp == 0) {
      return remove(subtreeRoot);
    } else if (cmp > 0) {
      subtreeRoot.left = remove(subtreeRoot.left, toRemove);
    } else {
      subtreeRoot.right = remove(subtreeRoot.right, toRemove);
    }

    subtreeRoot = updateNode(subtreeRoot);

    return subtreeRoot;
  }

  private AvlTreeMap.Node<K, V> remove(AvlTreeMap.Node<K, V> node) {
    // Easy if the node has 0 or 1 child.
    if (node.right == null) {
      return node.left;
    } else if (node.left == null) {
      return node.right;
    }

    // If it has two children, find the predecessor (max in left subtree),
    AvlTreeMap.Node<K, V> toReplaceWith = max(node);
    // then copy its data to the given node (value change),
    node.key = toReplaceWith.key;
    node.value = toReplaceWith.value;
    // then remove the predecessor node (structural change).
    node.left = remove(node.left, toReplaceWith);

    node = updateNode(node);

    return node;
  }

  @Override
  public V remove(K k) throws IllegalArgumentException {
    // TODO Implement Me!
    AvlTreeMap.Node<K, V> node = findForSure(k);
    V value = node.value;
    root = remove(root, node);
    size--;
    return value;
  }

  @Override
  public void put(K k, V v) throws IllegalArgumentException {
    // TODO Implement Me!
    AvlTreeMap.Node<K, V> n = findForSure(k);
    n.value = v;
  }

  @Override
  public V get(K k) throws IllegalArgumentException {
    // TODO Implement Me!
    AvlTreeMap.Node<K, V> n = findForSure(k);
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
   *
   * <p>Inner node class, each holds a key (which is what we sort the
   * BST by) as well as a value. We don't need a parent pointer as
   * long as we use recursive insert/remove helpers.</p>
   **/
  private static class Node<K, V> implements BinaryTreeNode {
    Node<K, V> left;
    Node<K, V> right;
    K key;
    V value;
    int height;
    int balanceFactor;

    // Constructor to make node creation easier to read.
    Node(K k, V v) {
      // left and right default to null
      key = k;
      value = v;
      height = 0;
    }

    @Override
    public String toString() {
      return key + ":" + value;
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

  // Iterative in-order traversal over the keys
  private class InorderIterator implements Iterator<K> {
    private final Stack<AvlTreeMap.Node<K, V>> stack;

    InorderIterator() {
      stack = new Stack<>();
      pushLeft(root);
    }

    private void pushLeft(AvlTreeMap.Node<K, V> curr) {
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
      AvlTreeMap.Node<K, V> top = stack.pop();
      pushLeft(top.right);
      return top.key;
    }
  }

}
