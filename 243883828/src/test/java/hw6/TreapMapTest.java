package hw6;

import hw6.bst.TreapMap;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

/**
 * In addition to the tests in BinarySearchTreeMapTest (and in OrderedMapTest & MapTest),
 * we add tests specific to Treap.
 */
@SuppressWarnings("All")
public class TreapMapTest extends BinarySearchTreeMapTest {

  @Override
  protected Map<String, String> createMap() {
    return new TreapMap<>(10);
  }

  // TODO Add tests
  //  (think about how you might write tests while randomness is involved in TreapMap implementation!)

  @Test
  @DisplayName("tests right rotation occurs after insert")
  public void insertRightRotation(){
    map.insert("7", "a");
    map.insert("5", "a");
    map.insert("3", "a"); //right rotation!

    String[] expected = new String[]{
            "7:a:-1157793070",
            "3:a:1107254586 null",
            "null 5:a:1913984760 null null"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  @Test
  @DisplayName("tests left rotation occurs after insert")
  public void insertLeftRotation(){
    map.insert("3", "a");
    map.insert("5", "a");
    map.insert("7", "a"); //left rotation!

    String[] expected = new String[]{
            "3:a:-1157793070",
            "null 7:a:1107254586",
            "null null 5:a:1913984760 null"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  @Test
  @DisplayName("tests right rotation followed by a left rotation after insertions")
  public void insertRightLeftRotation(){
    map.insert("3", "a");
    map.insert("1", "a");
    map.insert("4", "a");
    map.insert("6", "a");
    map.insert("5", "a"); // right then left rotation!


    String[] expected = new String[]{
            "3:a:-1157793070",
            "1:a:1913984760 5:a:254270492",
            "null null 4:a:1107254586 6:a:1773446580"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  @Test
  @DisplayName("tests left rotation followed by a left rotation after insertions")
  public void insertLeftRightRotation(){
    map.insert("4", "a");
    map.insert("5", "a");
    map.insert("3", "a");
    map.insert("1", "a");
    map.insert("2", "a");


    String[] expected = new String[]{
            "4:a:-1157793070",
            "2:a:254270492 5:a:1913984760",
            "1:a:1773446580 3:a:1107254586 null null"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  @Test
  @DisplayName("tests no rotation after insertion when already ordered")
  public void insertNoRotation(){
    map.insert("4", "a");
    map.insert("5", "a");
    map.insert("3", "a");


    String[] expected = new String[]{
            "4:a:-1157793070",
            "3:a:1107254586 5:a:1913984760"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }
  // no rotations should occur when there is only no children
  @Test
  @DisplayName("tests no rotation occurs after removal")
  public void removeNoRotation(){
    map.insert("5", "a");
    map.insert("6", "a");
    map.insert("4", "a");
    map.insert("3", "a");

    map.remove("6"); //no rotation

    String[] expected = new String[]{
            "5:a:-1157793070",
            "4:a:1107254586 null",
            "3:a:1773446580 null null null"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  @Test
  @DisplayName("tests right rotation occurs for removal on a node with one child")
  public void removeRightRotationOneChild(){
    map.insert("5", "a");
    map.insert("6", "a");
    map.insert("4", "a");
    map.insert("3", "a");

    map.remove("6"); //no rotation - already tested and passed
    map.remove("5");

    String[] expected = new String[]{
            "4:a:1107254586",
            "3:a:1773446580 null",
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  @Test
  @DisplayName("tests left rotation occurs for removal on a node with one child")
  public void removeLeftRotationOneChild(){
    map.insert("2", "a");
    map.insert("1", "a");
    map.insert("3", "a");
    map.insert("4", "a");

    map.remove("1"); //no rotation - already tested and passed
    map.remove("2"); //left rotation to remove

    String[] expected = new String[]{
            "3:a:1107254586",
            "null 4:a:1773446580"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  @Test
  @DisplayName("tests left then right rotation occurs for removal of a node with two children")
  public void removeLeftRightRotationTwoChildren(){
    map.insert("2", "a");
    map.insert("1", "a");
    map.insert("3", "a");

    map.remove("2"); // causes a left rotation then a right rotation to sink 2

    String[] expected = new String[]{
            "3:a:1107254586",
            "1:a:1913984760 null"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }

  @Test
  @DisplayName("tests right then left rotation occurs for removal of a node with two children")
  public void removeRightLeftRotationTwoChildren(){
    map.insert("2", "a");
    map.insert("3", "a");
    map.insert("1", "a");

    map.remove("2"); // causes a left rotation then a right rotation to sink 2

    String[] expected = new String[]{
            "1:a:1107254586",
            "null 3:a:1913984760"
    };
    assertEquals((String.join("\n", expected) + "\n"), map.toString());
  }
}