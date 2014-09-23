
#include <string.h>
#include <stdio.h>

#include "../include/jarvis.h"
#include "../util/util.h"

#include "Graph.h"
#include "jarvisHandles.h"

using namespace Jarvis;

jint Java_Graph_get_1id__LNode_2(JNIEnv *env, jobject graph, jobject node)
{
    Graph &j_db = *(getJarvisHandle<Graph>(env, graph));
    Node &j_node = *(getJarvisHandle<Node>(env, node));
    return j_db.get_id(j_node);
}

jint Java_Graph_get_1id__LEdge_2(JNIEnv *env, jobject graph, jobject edge)
{
    Graph &j_db = *(getJarvisHandle<Graph>(env, graph));
    Edge &j_edge = *(getJarvisHandle<Edge>(env, edge));
    return j_db.get_id(j_edge);
}

jobject JNICALL Java_Graph_get_1nodes__(JNIEnv *env, jobject graph)
{
    Graph &j_db = *(getJarvisHandle<Graph>(env, graph));
    try {
        NodeIterator *j_ni = new NodeIterator(j_db.get_nodes());
        jobject cur;

        if (*j_ni) {
            Node &j_n = **j_ni;
            cur = new_java_object(env, "Node", &j_n);
        }
        else
            cur = NULL;

        // create a java nodeiterator
        jclass cls = env->FindClass("NodeIterator");
        jmethodID cnstrctr = env->GetMethodID(cls, "<init>", "(JLNode;)V");
        jobject ni = env->NewObject(cls, cnstrctr,
                                    reinterpret_cast<jlong>(j_ni),
                                    cur);
        return ni;
    }
    catch (Exception e) {
        JavaThrow(env, e);
        return NULL;
    }
}


jobject JNICALL Java_Graph_get_1nodes__Ljava_lang_String_2(JNIEnv *env,
                    jobject graph, jstring tag)
{
    Graph &j_db = *(getJarvisHandle<Graph>(env, graph));
    const char *j_tag = tag != NULL ? env->GetStringUTFChars(tag, 0) : NULL;
    try {
        NodeIterator *j_ni = new NodeIterator(j_db.get_nodes(j_tag));
        jobject cur;

        if (*j_ni) {
            Node &j_n = **j_ni;
            cur = new_java_object(env, "Node", &j_n);
        }
        else
            cur = NULL;

        // create a java nodeiterator
        jclass cls = env->FindClass("NodeIterator");
        jmethodID cnstrctr = env->GetMethodID(cls, "<init>", "(JLNode;)V");
        jobject ni = env->NewObject(cls, cnstrctr,
                                    reinterpret_cast<jlong>(j_ni),
                                    cur);
        return ni;
    }
    catch (Exception e) {
        JavaThrow(env, e);
        return NULL;
    }
}


jobject JNICALL Java_Graph_get_1nodes__Ljava_lang_String_2LPropertyPredicate_2Z
    (JNIEnv *env, jobject graph, jstring tag, jobject pp, jboolean reverse)
{
    Graph &j_db = *(getJarvisHandle<Graph>(env, graph));
    const char *j_tag = tag != NULL ? env->GetStringUTFChars(tag, 0) : NULL;
    PropertyPredicate &j_pp = *(getJarvisHandle<PropertyPredicate>(env, pp));
    try {
        NodeIterator *j_ni = new NodeIterator(j_db.get_nodes(j_tag, j_pp, reverse));
        jobject cur;

        if (*j_ni) {
            Node &j_n = **j_ni;
            cur = new_java_object(env, "Node", &j_n);
        }
        else
            cur = NULL;

        //create a java nodeiterator
        jclass cls = env->FindClass("NodeIterator");
        jmethodID cnstrctr = env->GetMethodID(cls, "<init>", "(JLNode;)V");
        jobject ni = env->NewObject(cls, cnstrctr,
                                    reinterpret_cast<jlong>(j_ni),
                                    cur);
        return ni;
    }
    catch (Exception e) {
        JavaThrow(env, e);
        return NULL;
    }
}


jobject JNICALL Java_Graph_get_1edges__(JNIEnv *env, jobject graph)
{
    Graph &j_db = *(getJarvisHandle<Graph>(env, graph));
    try {
        EdgeIterator *j_ei = new EdgeIterator(j_db.get_edges());
        return new_java_object(env, "EdgeIterator", j_ei);
    }
    catch (Exception e) {
        JavaThrow(env, e);
        return NULL;
    }
}

jobject JNICALL Java_Graph_get_1edges__Ljava_lang_String_2(JNIEnv *env,
                    jobject graph, jstring tag)
{
    Graph &j_db = *(getJarvisHandle<Graph>(env, graph));
    const char *j_tag = tag != NULL ? env->GetStringUTFChars(tag, 0) : NULL;
    try {
        EdgeIterator *j_ei = new EdgeIterator(j_db.get_edges(j_tag));
        return new_java_object(env, "EdgeIterator", j_ei);
    }
    catch (Exception e) {
        JavaThrow(env, e);
        return NULL;
    }
}

jobject JNICALL Java_Graph_get_1edges__Ljava_lang_String_2LPropertyPredicate_2Z
    (JNIEnv *env, jobject graph, jstring tag, jobject pp, jboolean reverse)
{
    Graph &j_db = *(getJarvisHandle<Graph>(env, graph));
    const char *j_tag = tag != NULL ? env->GetStringUTFChars(tag, 0) : NULL;
    PropertyPredicate &j_pp = *(getJarvisHandle<PropertyPredicate>(env, pp));
    try {
        EdgeIterator *j_ei = new EdgeIterator(j_db.get_edges(j_tag, j_pp, reverse));
        return new_java_object(env, "EdgeIterator", j_ei);
    }
    catch (Exception e) {
        JavaThrow(env, e);
        return NULL;
    }
}

void Java_Graph_loadGraphNative(JNIEnv *env, jobject obj,
                                jstring filename, jint options)
{
    const char *db_name = env->GetStringUTFChars(filename, 0);
    try {
        Graph *db = new Graph(db_name, options);
        setJarvisHandle(env, obj, db);
    }
    catch (Exception e) {
        JavaThrow(env, e);
    }
    env->ReleaseStringUTFChars(filename, db_name);
}

jobject JNICALL Java_Graph_add_1node(JNIEnv *env, jobject graph, jstring tag)
{
    Graph &j_db = *(getJarvisHandle<Graph>(env, graph));
    const char *j_tag = tag != NULL ? env->GetStringUTFChars(tag, 0) : NULL;
    try {
        Node &j_node = j_db.add_node(j_tag);
        return new_java_object(env, "Node", &j_node);
    }
    catch (Exception e) {
        JavaThrow(env, e);
        return NULL;
    }
}

jobject JNICALL Java_Graph_add_1edge(JNIEnv *env, jobject graph,
                                     jobject src, jobject dest, jstring tag)
{
    Graph &j_db = *(getJarvisHandle<Graph>(env, graph));
    Node &j_src = *(getJarvisHandle<Node>(env, src));
    Node &j_dest = *(getJarvisHandle<Node>(env, dest));
    const char *j_tag = tag != NULL ? env->GetStringUTFChars(tag, 0) : NULL;
    try {
        Edge &j_edge = j_db.add_edge(j_src, j_dest, j_tag);
        return new_java_object(env, "Edge", &j_edge);
    }
    catch (Exception e) {
        JavaThrow(env, e);
        return NULL;
    }
}

void Java_Graph_remove__LNode_2(JNIEnv *env, jobject graph, jobject node)
{
    Graph &j_db = *(getJarvisHandle<Graph>(env, graph));
    Node &j_node = *(getJarvisHandle<Node>(env, node));
    try {
        j_db.remove(j_node);
    }
    catch (Exception e) {
        JavaThrow(env, e);
    }
}

void Java_Graph_remove__LEdge_2(JNIEnv *env, jobject graph, jobject edge)
{
    Graph &j_db = *(getJarvisHandle<Graph>(env, graph));
    Edge &j_edge = *(getJarvisHandle<Edge>(env, edge));
    try {
        j_db.remove(j_edge);
    }
    catch (Exception e) {
        JavaThrow(env, e);
    }
}

void Java_Graph_dispose(JNIEnv *env, jobject graph)
{
    Graph *j_db = getJarvisHandle<Graph>(env, graph);
    delete j_db;
    setJarvisHandle(env, graph, static_cast<Graph *>(NULL));
}

jobject new_java_object(JNIEnv *env, const char *name, void *obj)
{
    jclass cls = env->FindClass(name);
    jmethodID cnstrctr = env->GetMethodID(cls, "<init>", "(J)V");
    return env->NewObject(cls, cnstrctr, reinterpret_cast<jlong>(obj));
}
