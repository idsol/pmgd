/*
 * Corresponds to the node.h file in Jarvis.
 */

package jarvis;

public class Node {
    private long jarvisHandle;
    private Node(long handle) { jarvisHandle = handle; }

    public enum Direction { ANY, OUTGOING, INCOMING };

    public native String get_tag() throws Exception;
    public native Property get_property(String property) throws Exception;
    public native PropertyIterator get_properties() throws Exception;
    public native EdgeIterator get_edges() throws Exception;
    public native EdgeIterator get_edges(String tag) throws Exception;
    public EdgeIterator get_edges(Direction dir) throws Exception
        { return get_edges(dir.ordinal()); }
    public EdgeIterator get_edges(Direction dir, String tag) throws Exception
        { return get_edges(dir.ordinal(), tag); }
    private native EdgeIterator get_edges(int dir) throws Exception;
    private native EdgeIterator get_edges(int dir, String tag) throws Exception;
    public native void set_property(String id, Property prop) throws Exception;
    public native void remove_property(String name) throws Exception;

    public NodeIterator get_neighbors()
        { return get_neighbors(true); }
    public native NodeIterator get_neighbors(boolean unique);

    public NodeIterator get_neighbors(Direction dir)
        { return get_neighbors(dir.ordinal(), true); }
    public NodeIterator get_neighbors(Direction dir, boolean unique)
        { return get_neighbors(dir.ordinal(), unique); }
    private native NodeIterator get_neighbors(int dir, boolean unique);

    public NodeIterator get_neighbors(String tag)
        { return get_neighbors(tag, true); }
    public native NodeIterator get_neighbors(String tag, boolean unique);

    public NodeIterator get_neighbors(Direction dir, String tag)
        { return get_neighbors(dir.ordinal(), tag, true); }
    public NodeIterator get_neighbors(Direction dir, String tag, boolean unique)
        { return get_neighbors(dir.ordinal(), tag, unique); }
    private native NodeIterator get_neighbors(int dir, String tag, boolean unique);
}