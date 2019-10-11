//
// LDBC query 2
//
#include <string>
#include <list>
#include "pmgd.h"
#include "util.h"
#include "neighbor.h"
#include "query2.h"
#include "strings.h"

using namespace PMGD;

class Result
{
    static const int MAX_RESULTS = 20;

    Query2Result results;

public:
    Query2Result get_results() { return results; }

    void add(const Node &person, const Node &post)
    {
        Query2ResultItem r(person, post);

        if (results.size() < MAX_RESULTS || r < results.back()) {
            std::list<Query2ResultItem>::iterator pos = results.begin();
            while (pos != results.end() && *pos < r)
                pos++;
            results.insert(pos, r);

            if (results.size() > MAX_RESULTS)
                results.pop_back();
        }
    }

    void fill()
    {
        for (auto &r : results)
            r.fill();
    }
};

Query2Result query2(Graph &db, unsigned long long id, unsigned long long date)
{
    strings.init();

    time_t t = date / 1000;
    unsigned long ms = date % 1000;
    PMGD::Time end_time(gmtime(&t), ms * 1000, 0, 0);

    Node &start = *db.get_nodes(strings.Person, PropertyPredicate(strings.id, PropertyPredicate::Eq, id));

    Result result;

    get_neighbors(start, Any, strings.knows, false)
        .process([end_time, &result](Node &person) {
            get_neighbors(person, Incoming, strings.hasCreator, false)
                .process([&person, end_time, &result](Node &post) {
                        if (post.get_property(strings.creationDate).time_value() <= end_time)
                            result.add(person, post);
                    });
        });

    result.fill();

    return result.get_results();
}


Query2ResultItem::Query2ResultItem(const Node &person, const Node &post)
    : _person(person),
      _post(post)
{
    post_id = post.get_property(strings.id).int_value();
    creationDate = post.get_property(strings.creationDate).time_value();
}

void Query2ResultItem::fill()
{
    friend_id = _person.get_property(strings.id).int_value();
    firstName = _person.get_property(strings.firstName).string_value();
    lastName = _person.get_property(strings.lastName).string_value();

    Property content_property;
    if (!(_post.check_property(strings.content, content_property)
            && (content = content_property.string_value()) != ""))
        content = _post.get_property(strings.imageFile).string_value();
}

bool Query2ResultItem::operator<(const Query2ResultItem &a) const
{
    if (creationDate == a.creationDate)
        return post_id < a.post_id;
    return creationDate > a.creationDate;
}


#ifdef MAIN
#include <stdio.h>
#include "args.h"
#include "timing.h"

static void print(const Query2Result &);
static void print(const Query2ResultItem &);

strings_t strings;

int main(int argc, char **argv)
{
    GET_ARGS

    FILE *f = fopen(filename, "r");
    assert(f != NULL);

    char s[40];
    char *r = fgets(s, sizeof s, f);
    assert(r != NULL);
    assert(strcmp(s, "Person|Date0\n") == 0);

    try {
        Graph db(graph_name, Graph::ReadOnly);

        START_TIMING

        char s[40];
        while (fgets(s, sizeof s, f) != NULL) {
            Transaction tx(db);
            char *e;
            unsigned long long id = strtoull(s, &e, 10);
            assert(*e == '|');
            time_t date = strtoull(e + 1, &e, 10) / 1000; // convert ms to s
            assert(*e == '\n');
            if (verbose) {
                char date_str[40];
                strftime(date_str, sizeof date_str,
                         "%a %b %d %H:%M:%S %Y", gmtime(&date));
                printf("%llu %s\n", id, date_str);
            }

            Query2Result result = query2(db, id, date);

            if (verbose) print(result);
        }

        END_TIMING
    }
    catch (Exception e) {
        print_exception(e);
        return 1;
    }

    return 0;
}

static void print(const Query2Result &results)
{
    if (results.empty())
        printf("  no matches\n");
    else
        for (const auto &r : results)
            print(r);
    printf("\n");
}

static void print(const Query2ResultItem &r)
{
    printf("  %llu %s %s %llu %s %s\n",
            r.friend_id, r.firstName.c_str(), r.lastName.c_str(), r.post_id,
            time_to_string(r.creationDate).c_str(), r.content.c_str());
}
#endif