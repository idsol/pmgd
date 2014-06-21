/*
 * This test checks Jarvis property lists
 */

#include "jarvis.h"
#include "../src/IndexManager.h"
#include "../util/util.h"

using namespace Jarvis;

int main(int argc, char **argv)
{
    int node_count = 8;
    int edge_count = node_count - 1;

    printf("node_count = %d\n", node_count);

    try {
        Graph db("indexgraph", Graph::Create);

        Node **nodes = new Node *[node_count + 1];
        Transaction tx(db);

        for (int i = 1; i <= 2; i++) {
            db.create_index(Graph::NODE, "tag1", "id1", t_integer);
            Node &n = db.add_node("tag1");
            n.set_property("id1", i + 1611);
            nodes[i] = &n;
        }
        for (int i = 3; i <= 4; i++) {
            Node &n = db.add_node("tag1");
            // More nodes with the same property value and tag
            n.set_property("id1", i - 2 + 1611);
            nodes[i] = &n;
        }
        for (int i = 5; i <= 6; i++) {
            db.create_index(Graph::NODE, "tag1", "id2", t_float);
            Node &n = db.add_node("tag1");
            n.set_property("id2", i + 23.57);
            nodes[i] = &n;
        }
        try {
            printf("***Testing bad set property\n");
            for (int i = 7; i <= 7; i++) {
                Node &n = db.add_node("tag1");
                nodes[i] = &n;
                n.set_property("id2", true);
            }
        } catch (Exception e) {
            print_exception(e);
        }
        for (int i = 8; i <= node_count; i++) {
            db.create_index(Graph::NODE, "tag2", "id1", t_float);
            Node &n = db.add_node("tag2");
            n.set_property("id1", i + 23.57);
            nodes[i] = &n;
        }

        for (int i = 1; i <= edge_count; i++) {
            Edge &e = db.add_edge(*nodes[i], *nodes[i+1], 0);
            e.set_property("id", i + 2611);
        }
        dump_nodes(db);
        dump_edges(db);

        printf("## Trying iterator with tag tag1 and property id1:1612\n");
        PropertyPredicate pp("id1", PropertyPredicate::eq, 1612);
        for (NodeIterator i = db.get_nodes("tag1", pp); i; i.next()) {
            printf("Node %lu: tag %s\n", db.get_id(*i), i->get_tag().name().c_str());
            printf("\tConfirming searched prop value: %lld\n", i->get_property("id1").int_value());
        }

        printf("## Test set_property involving a remove and add\n");
        nodes[1]->set_property("id1", 2433);
        printf("Property id1 for node 1: %lld\n", nodes[1]->get_property("id1").int_value());

        printf("## Trying iterator with tag tag1 and property id1:1612 after changing value of one node\n");
        for (NodeIterator i = db.get_nodes("tag1", pp); i; i.next()) {
            printf("Node %lu: tag %s\n", db.get_id(*i), i->get_tag().name().c_str());
            printf("\tConfirming searched prop value: %lld\n", i->get_property("id1").int_value());
        }

        printf("## Trying iterator with tag tag1 and property id1:2433 after changing value of one node\n");
        pp.v1 = 2433;
        for (NodeIterator i = db.get_nodes("tag1", pp); i; i.next()) {
            printf("Node %lu: tag %s\n", db.get_id(*i), i->get_tag().name().c_str());
            printf("\tConfirming searched prop value: %lld\n", i->get_property("id1").int_value());
        }

        printf("## Trying iterator with just tag\n");
        for (NodeIterator i = db.get_nodes("tag1"); i; i.next()) {
            printf("Node %lu: tag %s\n", db.get_id(*i), i->get_tag().name().c_str());
        }

        printf("## Trying iterator with tag and float prop\n");
        PropertyPredicate ppf("id1", PropertyPredicate::eq, 31.57);
        for (NodeIterator i = db.get_nodes("tag2", ppf); i; i.next()) {
            printf("Node %lu: tag %s\n", db.get_id(*i), i->get_tag().name().c_str());
            printf("\tConfirming searched prop value: %lf\n", i->get_property("id1").float_value());
        }

        printf("## Trying NULL iterator with missing property\n");
        ppf.v1 = 34.8;
        for (NodeIterator i = db.get_nodes("tag2", ppf); i; i.next()) {
            printf("Node %lu: tag %s\n", db.get_id(*i), i->get_tag().name().c_str());
        }

        printf("## Trying NULL iterator with tag3\n");
        for (NodeIterator i = db.get_nodes("tag3"); i; i.next()) {
            printf("Node %lu: tag %s\n", db.get_id(*i), i->get_tag().name().c_str());
        }
        
        printf("## Trying plain get_nodes() iterator\n");
        for (NodeIterator i = db.get_nodes(); i; i.next()) {
            printf("Node %lu: tag %s\n", db.get_id(*i), i->get_tag().name().c_str());
        }
        tx.commit();
    }
    catch (Exception e) {
        print_exception(e);
        return 1;
    }

    return 0;
}