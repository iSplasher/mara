
#include "include/test.h"

#include <map>
#include <string>
#include <utility>

#include "../include/tree.h"


using namespace NAMESPACE;


static test::suite _ = [] {

    using namespace test;
    using namespace test::bdd;

    "Tree is well defined"_test = []() {

        given("A a tree to print") = []() {
            int root = 0;

            Tree<int> tree(std::move(root));

            when("We add children") = [&]() {
                for (int i = 1; i < 10; ++i) {
                    auto c = i;
                    tree.add_child(std::move(c));
                }
                then("We get the correct number of children") = [&]() {
                    expect(tree.unsafe_children_size() == 9_i);
                };
                // THEN( "We get the correct children" ) {
                //   expect( tree.children().front() == 1 );
                //   expect( tree.children().back() == 9 );
                // }
            };
        };

        given("A tree with a single root") = [&]() {
            int root = 0;

            Tree<int> tree(std::move(root));

            when("We add a single child") = [&]() {

                auto it1 = tree.add_child(1);

                then("The tree has no siblings because it's root") = [&]() {
                    expect(tree.unsafe_siblings_size() == 1_i);
                };

                then("The tree has two elements (with root)") = [&]() {
                    expect(tree.unsafe_size() == 2_i);
                };

                then("The tree has one child") = [&]() {
                    expect(tree.unsafe_children_size() == 1_i);
                };

                then("We can iterate the tree from root") = [&]() {

                    auto it = tree.unsafe_iter();

                    auto vals = std::forward_list({0, 1});

                    int i = 0;
                    while (tree.is_valid(it)) {
                        i++;
                        const auto v = *it;
                        expect(vals.front() == v);
                        vals.pop_front();
                        it++;
                    }

                    expect(i == _i(tree.unsafe_size()));
                };

                then("We can iterate the children tree root inclusive") = [&]() {

                    auto it = tree.unsafe_iter();

                    std::map<int, bool> vals;

                    for (auto i = 0; i != tree.unsafe_size(); ++i) {
                        vals[i] = false;
                    }

                    while (tree.is_valid(it)) {
                        const auto v = *it;
                        vals[v] = true;
                        it++;
                    }

                    for (auto i = 0; i != tree.unsafe_size(); ++i) {
                        expect(_b(vals[i]) == true);
                    }
                };

                then("We can iterate the children tree root exclusive") = [&]() {
                    auto it = tree.unsafe_iter_children();

                    std::map<int, bool> vals;

                    for (auto i = 1; i != tree.unsafe_size(); ++i) {
                        vals[i] = false;
                    }

                    while (tree.is_valid(it)) {
                        const auto v = *it;
                        vals[v] = true;
                        it++;
                    }

                    for (auto i = 1; i != tree.unsafe_size(); ++i) {
                        const auto v = vals[i];
                        expect(_b(v) == true);
                    }
                };
            };

            then("No children") = [&]() {
                expect(tree.unsafe_children_size() == 0_i);
            };

            when("We add many children") = [&]() {

                for (auto i = 1; i != 11; ++i) {
                    auto c = i;
                    tree.add_child(std::move(c));
                }

                then("We get the correct number of children") = [&]() {
                    expect(tree.unsafe_children_size() == 10_i);
                };

                then("We get the correct number of direct children") = [&]() {
                    expect(tree.unsafe_depth_size() == 10_i);
                };

                then("We can iterate breadth_first children tree root exclusive") = [&]() {
                    auto it = tree.unsafe_iter_children();

                    auto vals = std::forward_list({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

                    int i = 0;
                    do {
                        i++;
                        const auto v = *it;
                        expect(vals.front() == v);
                        vals.pop_front();
                    } while (it != it.end());

                    expect(i == _i(tree.unsafe_size() - 1));
                };

                then("We can iterate breadth_first children tree root inclusive") = [&]() {
                    auto it = tree.unsafe_iter();

                    auto vals = std::forward_list({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

                    int i = 0;
                    while (tree.is_valid(it)) {
                        i++;
                        const auto v = *it;
                        expect(vals.front() == v);
                        vals.pop_front();
                        it++;
                    }

                    expect(i == _i(tree.unsafe_size()));
                };

                then("We can print the tree") = [&]() {

                    auto str = print_tree(tree);

                    expect(str == " ");
                    expect(str.empty() == false);
                };
            };

            when("We construct a tree of children") = [&]() {

                const int depth = 5;
                const int children = 3;

                // construct a tree of depth 5 with 3 children using tree.add_child()
                auto root_tree = tree.at(tree.root());
                for (auto i = 0; i != depth; ++i) {
                    auto it = root_tree.unsafe_iter_depth();
                    for (auto j = 0; j != children; ++j) {
                        auto c = i * children + j;
                        auto new_tree = root_tree.add_child(std::move(c));
                        ++it;
                        if (tree.is_valid(it)) {
                            root_tree = root_tree.at(it);
                        }
                        if (j == children - 1) {
                            root_tree = std::move(new_tree.value());
                        }
                    }
                };
                then("We get the correct number of children") = [&]() {
                    expect(_i(tree.unsafe_children_size()) == (depth * children));
                };
                then("We get the correct number of direct children") = [&]() {
                    expect(_i(tree.unsafe_depth_size()) == (depth * children));
                };


                then("We can iterate breadth_first children tree root exclusive") = [&]() {
                    auto it = tree.unsafe_iter_children();

                    auto vals = std::forward_list({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19});
                    int i = 0;
                    while (tree.is_valid(it)) {
                        i++;
                        const auto v = *it;
                        expect(vals.front() == v);
                        vals.pop_front();
                        it++;
                    }

                    expect(i == _i(tree.unsafe_size() - 1));
                };

                then("We can iterate breadth_first children tree root inclusive") = [&]() {
                    auto it = tree.unsafe_iter();

                    auto vals = std::forward_list({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19});
                    int i = 0;
                    while (tree.is_valid(it)) {
                        i++;
                        const auto v = *it;
                        expect(vals.front() == v);
                        vals.pop_front();
                        it++;
                    }

                    expect(i == _i(tree.unsafe_size()));
                };

                then("We can print the tree") = [&]() {
                    auto str = print_tree(tree);
                    expect(str == " ");
                    expect(str.empty() == false);
                };
            };
        };
    };
};