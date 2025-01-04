package hw6;

import hw6.bst.AvlTreeMap;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

/**
 * In addition to the tests in BinarySearchTreeMapTest (and in OrderedMapTest & MapTest),
 * we add tests specific to AVL Tree.
 */
@SuppressWarnings("All")
public class AvlTreeMapTest extends BinarySearchTreeMapTest {

  @Override
  protected Map<String, String> createMap() {
    return new AvlTreeMap<>();
  }

  @Test
  @DisplayName("tests left rotation when right heavy after an insertion")
  public void insertLeftRotation() {
    map.insert("1", "a");
    // System.out.println(avl.toString());
    // must print
    /*
        1:a
     */

    map.insert("2", "b");
    // System.out.println(avl.toString());
    // must print
    /*
        1:a,
        null 2:b
     */

    map.insert("3", "c"); // it must do a left rotation here!
    // System.out.println(avl.toString());
    // must print
    /*
        2:b,
        1:a 3:c
     */

    String[] expected = new String[]{
        "2:b",
        "1:a 3:c"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  // TODO Add more tests

  @Test
  @DisplayName("tests right rotation when left heavy after an insertion")
  public void insertRightRotation() {
    map.insert("3", "a");
    map.insert("2", "b");
    map.insert("1", "c"); // it must do a right rotation here!

    String[] expected = new String[]{
            "2:b",
            "1:c 3:a"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  @Test
  @DisplayName("tests right left rotation when there is a left heavy zig zag after an insertion")
  public void insertRightLeftRotation() {
    map.insert("3", "a");
    map.insert("7", "b");
    map.insert("5", "c"); // it must do a right-left rotation here!

    String[] expected = new String[]{
            "5:c",
            "3:a 7:b"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  @Test
  @DisplayName("tests left right rotation when there is a right heavy zig zag after an insertion")
  public void insertLeftRightRotation() {
    map.insert("7", "a");
    map.insert("3", "b");
    map.insert("5", "c"); // it must do a left-right rotation here!

    String[] expected = new String[]{
            "5:c",
            "3:b 7:a"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  @Test
  @DisplayName("tests right rotation on left sub-tree after insert")
  public void insertRightRotationRighttSubTree() {
    map.insert("3", "a");
    map.insert("9", "b");
    map.insert("2", "c");
    map.insert("1", "d");
    map.insert("8", "e");
    map.insert("7", "f"); // must do a right rotation here!

    String[] expected = new String[]{
            "3:a",
            "2:c 8:e",
            "1:d null 7:f 9:b"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  @Test
  @DisplayName("tests left rotation on right sub-tree after insert")
  public void insertLeftRotationLeftSubTree() {
    map.insert("7", "a");
    map.insert("4", "b");
    map.insert("8", "c");
    map.insert("5", "d");
    map.insert("9", "e");
    map.insert("6", "f"); // must do a left rotation here!

    String[] expected = new String[]{
            "7:a",
            "5:d 8:c",
            "4:b 6:f null 9:e"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  @Test
  @DisplayName("tests no rotation is called when tree is already balanced after insertion")
  public void insertNoRotation() {
    map.insert("2", "a");
    map.insert("1", "b");
    map.insert("3", "c"); // balanced!

    String[] expected = new String[]{
            "2:a",
            "1:b 3:c"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  @Test
  @DisplayName("tests left rotation when right heavy after a removal")
  public void removeLeftRotation() {
    map.insert("4", "a");
    map.insert("3", "b");
    map.insert("6", "c");
    map.insert("2", "d");
    map.insert("5", "e");
    map.insert("7", "f");
    map.insert("8", "g"); // balanced!

    map.remove("5"); // must do left rotation!

    String[] expected = new String[]{
            "4:a",
            "3:b 7:f",
            "2:d null 6:c 8:g"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  @Test
  @DisplayName("tests right rotation when left heavy after a removal")
  public void removeRightRotation() {
    map.insert("7", "a");
    map.insert("5", "b");
    map.insert("8", "c");
    map.insert("4", "d");
    map.insert("6", "e");
    map.insert("9", "f");
    map.insert("3", "g"); // balanced!

    map.remove("6"); // must do right rotation!

    String[] expected = new String[]{
            "7:a",
            "4:d 8:c",
            "3:g 5:b null 9:f"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  @Test
  @DisplayName("tests right-left rotation after removal")
  public void removeRightLeftRotation() {
    map.insert("5", "a");
    map.insert("4", "b");
    map.insert("8", "c");
    map.insert("3", "d");
    map.insert("6", "e");
    map.insert("9", "f");
    map.insert("7", "g"); // balanced!

    map.remove("3"); // must do left-right rotation!

    String[] expected = new String[]{
            "6:e",
            "5:a 8:c",
            "4:b null 7:g 9:f"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  @Test
  @DisplayName("tests left-right rotation after removal")
  public void removeLeftRightRotation() {
    map.insert("l", "a");
    map.insert("i", "b");
    map.insert("m", "c");
    map.insert("h", "d");
    map.insert("j", "e");
    map.insert("n", "f");
    map.insert("k", "g"); // balanced!

    map.remove("n"); // must do left-right rotation!

    String[] expected = new String[]{
            "j:e",
            "i:b l:a",
            "h:d null k:g m:c"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  @Test
  @DisplayName("tests no rotation is called when tree is already balanced after removal")
  public void removeNoRotation() {
    map.insert("2", "a");
    map.insert("1", "b");
    map.insert("3", "c"); // balanced!

    map.remove("3"); // still balanced!

    String[] expected = new String[]{
            "2:a",
            "1:b null"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  @Test
  @DisplayName("tests right rotation after right rotation inserts")
  public void insertRightRight(){
    map.insert("7", "a");
    map.insert("5", "b");
    map.insert("4", "c"); // right rotation!
    map.insert("3", "d");
    map.insert("2", "e"); //right rotation!

    String[] expected = new String[]{
            "5:b",
            "3:d 7:a",
            "2:e 4:c null null"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());

  }

  @Test
  @DisplayName("tests left rotation after left rotation inserts")
  public void insertLeftLeft(){
    map.insert("1", "a");
    map.insert("2", "b");
    map.insert("3", "c"); // left rotation!
    map.insert("4", "d");
    map.insert("5", "e"); //left rotation!

    String[] expected = new String[]{
            "2:b",
            "1:a 4:d",
            "null null 3:c 5:e"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());

  }

  @Test
  @DisplayName("tests right-left rotation then right rotation inserts")
  public void insertRightlefttRight(){
    map.insert("3", "a");
    map.insert("7", "b");
    map.insert("5", "c"); // right-left rotation!
    map.insert("2", "d");
    map.insert("1", "e"); //right rotation!

    String[] expected = new String[]{
            "5:c",
            "2:d 7:b",
            "1:e 3:a null null"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());

  }

  @Test
  @DisplayName("tests right rotation then right-left rotation inserts")
  public void insertRightRightleft(){
    map.insert("6", "a");
    map.insert("3", "b");
    map.insert("2", "c"); // right rotation!
    map.insert("8", "d");
    map.insert("7", "e"); //right-left rotation!

    String[] expected = new String[]{
            "3:b",
            "2:c 7:e",
            "null null 6:a 8:d"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());

  }

  @Test
  @DisplayName("tests left rotation then right-left rotation inserts")
  public void insertLeftRightleft(){
    map.insert("5", "a");
    map.insert("6", "b");
    map.insert("7", "c"); // left rotation!
    map.insert("9", "d");
    map.insert("8", "e"); //right-left rotation!

    String[] expected = new String[]{
            "6:b",
            "5:a 8:e",
            "null null 7:c 9:d"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());

  }

  @Test
  @DisplayName("tests right-left rotation then left rotation inserts")
  public void insertRightleftLeft(){
    map.insert("1", "a");
    map.insert("5", "b");
    map.insert("4", "c"); // rigt-left rotation!
    map.insert("2", "d");
    map.insert("3", "e"); //left rotation!

    String[] expected = new String[]{
            "4:c",
            "2:d 5:b",
            "1:a 3:e null null"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());

  }

  @Test
  @DisplayName("tests right rotation then left-right rotation inserts")
  public void insertRightLeftright(){
    map.insert("5", "a");
    map.insert("4", "b");
    map.insert("3", "c"); // right rotation!
    map.insert("1", "d");
    map.insert("2", "e"); //left-right rotation!

    String[] expected = new String[]{
            "4:b",
            "2:e 5:a",
            "1:d 3:c null null"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());

  }

  @Test
  @DisplayName("tests left-right rotation then right rotation inserts")
  public void insertLeftrightRight(){
    map.insert("8", "a");
    map.insert("4", "b");
    map.insert("5", "c"); // left-right rotation!
    map.insert("7", "d");
    map.insert("6", "e"); //right rotation!

    String[] expected = new String[]{
            "5:c",
            "4:b 7:d",
            "null null 6:e 8:a"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());

  }

  @Test
  @DisplayName("tests left-right rotation then left rotation inserts")
  public void insertLeftrightLeft(){
    map.insert("5", "a");
    map.insert("1", "b");
    map.insert("4", "c"); // left-right rotation!
    map.insert("2", "d");
    map.insert("3", "e"); //left rotation!

    String[] expected = new String[]{
            "4:c",
            "2:d 5:a",
            "1:b 3:e null null"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());

  }

  @Test
  @DisplayName("tests left rotation then left-right rotation inserts")
  public void insertLeftLeftright(){
    map.insert("4", "a");
    map.insert("5", "b");
    map.insert("8", "c"); // left rotation!
    map.insert("6", "d");
    map.insert("7", "e"); //right rotation!

    String[] expected = new String[]{
            "5:b",
            "4:a 7:e",
            "null null 6:d 8:c"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());

  }

  @Test
  @DisplayName("tests right rotation then right-left rotation removals")
  public void removeRightRightleft(){
    map.insert("f", "a");
    map.insert("b", "b");
    map.insert("h", "c");
    map.insert("a", "d");
    map.insert("d", "e");
    map.insert("g", "f");
    map.insert("j", "g");
    map.insert("c", "h");
    map.insert("e", "i");
    map.insert("i", "j");
    map.insert("k", "k"); // balanced

    map.remove("i");
    map.remove("c");
    map.remove("e");
    map.remove("a");
    map.remove("d");
    map.remove("g");//right rotation!

    map.remove("b"); //right-left rotation!

    String[] expected = new String[]{
            "h:c",
            "f:a j:g",
            "null null null k:k"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());

  }

  @Test
  @DisplayName("tests right rotation then right-left rotation removals")
  public void removeRightRight(){
    map.insert("f", "a");
    map.insert("b", "b");
    map.insert("h", "c");
    map.insert("a", "d");
    map.insert("d", "e");
    map.insert("g", "f");
    map.insert("j", "g");
    map.insert("c", "h");
    map.insert("e", "i");
    map.insert("i", "j");
    map.insert("k", "k"); // balanced

    map.remove("i");
    map.remove("c");
    map.remove("e");
    map.remove("a");
    map.remove("d");
    map.remove("g");//right rotation!

    map.remove("b"); //right-left rotation!

    String[] expected = new String[]{
            "h:c",
            "f:a j:g",
            "null null null k:k"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());

  }

  @Test
  @DisplayName("tests right rotation when left heavy after a removal with one child")
  public void removeRightRotationOneChild() {
    map.insert("3", "a");
    map.insert("2", "b");
    map.insert("5", "c");
    map.insert("1", "d");
    map.insert("4", "e");
    map.insert("6", "f");
    map.insert("7", "g"); // balanced!

    map.remove("2"); // must do right rotation!

    String[] expected = new String[]{
            "5:c",
            "3:a 6:f",
            "1:d 4:e null 7:g"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  @Test
  @DisplayName("tests left rotation when right heavy after a removal with one child")
  public void removeLeftRotationOneChild() {
    map.insert("5", "a");
    map.insert("3", "b");
    map.insert("6", "c");
    map.insert("2", "d");
    map.insert("4", "e");
    map.insert("7", "f");
    map.insert("1", "g"); // balanced!

    map.remove("6"); // must do left rotation!

    String[] expected = new String[]{
            "3:b",
            "2:d 5:a",
            "1:g null 4:e 7:f"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  @Test
  @DisplayName("tests replaces with the maximum in left subtree when removing root")
  public void removeRootTwoChildren() {
    map.insert("2", "a");
    map.insert("1", "b");
    map.insert("3", "c");

    map.remove("2"); // balanced!

    String[] expected = new String[]{
            "1:b",
            "null 3:c"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  @Test
  @DisplayName("tests right rotation when left heavy after a removal with two children")
  public void removeRightRotationTwoChildren() {
    map.insert("5", "a");
    map.insert("3", "b");
    map.insert("6", "c");
    map.insert("2", "d");
    map.insert("4", "e");
    map.insert("7", "f");
    map.insert("1", "g"); // balanced!

    map.remove("3"); // must do right rotation!

    String[] expected = new String[]{
            "5:a",
            "2:d 6:c",
            "1:g 4:e null 7:f"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  @Test
  @DisplayName("tests left rotation when right heavy after a removal with two children")
  public void removeLeftRotationTwoChildren() {
    map.insert("3", "a");
    map.insert("2", "b");
    map.insert("5", "c");
    map.insert("1", "d");
    map.insert("4", "e");
    map.insert("6", "f");
    map.insert("7", "g"); // balanced!

    map.remove("5"); // must do left rotation!

    String[] expected = new String[]{
            "3:a",
            "2:b 6:f",
            "1:d null 4:e 7:g"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  @Test
  @DisplayName("tests left-right rotation when left heavy after a removal with one child")
  public void removeLeftRightRotationOneChild() {
    map.insert("4", "a");
    map.insert("3", "b");
    map.insert("5", "c");
    map.insert("1", "d");
    map.insert("6", "e");
    map.insert("2", "f"); // balanced!

    map.remove("5"); // must do left-right rotation!

    String[] expected = new String[]{
            "4:a",
            "2:f 6:e",
            "1:d 3:b null null"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  @Test
  @DisplayName("tests right-left rotation when right heavy after a removal with one child")
  public void removeRightLeftRotationOneChild() {
    map.insert("3", "a");
    map.insert("2", "b");
    map.insert("4", "c");
    map.insert("1", "d");
    map.insert("6", "e");
    map.insert("5", "f"); // balanced!

    map.remove("2"); // must do right-left rotation!

    String[] expected = new String[]{
            "3:a",
            "1:d 5:f",
            "null null 4:c 6:e"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  @Test
  @DisplayName("tests left-right rotation when left heavy after a removal with two children")
  public void removeLeftRightRotationTwoChildren() {
    map.insert("5", "a");
    map.insert("3", "b");
    map.insert("6", "c");
    map.insert("1", "d");
    map.insert("4", "e");
    map.insert("7", "f");
    map.insert("2", "g"); // balanced!

    map.remove("3"); // must do left-right rotation!

    String[] expected = new String[]{
            "5:a",
            "2:g 6:c",
            "1:d 4:e null 7:f"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  @Test
  @DisplayName("tests right left rotation when right heavy after a removal with two children")
  public void removeRightLeftRotationTwoChildren() {
    map.insert("3", "a");
    map.insert("2", "b");
    map.insert("5", "c");
    map.insert("1", "d");
    map.insert("4", "e");
    map.insert("7", "f");
    map.insert("6", "g"); // balanced!

    map.remove("5"); // must do right-left rotation!

    String[] expected = new String[]{
            "3:a",
            "2:b 6:g",
            "1:d null 4:e 7:f"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  @Test
  @DisplayName("complex situation of multiple inserts and removals at random")
  public void insertDoubleDoubleRotation() {

  }

}
