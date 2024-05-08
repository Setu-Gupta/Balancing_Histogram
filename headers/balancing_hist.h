#pragma once

#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <utility>
#include <vector>

template<typename T>
concept UsableType = requires(T v)
{
        std::numeric_limits<T>::is_specialized;
        {v / 2} -> std::same_as<T>;
};

namespace balancing_histogram
{
        template<UsableType T, std::size_t N>
        class hist
        {
                private:
                        struct histogram_bin
                        {
                                        bool                           leaf;
                                        std::map<T, std::size_t>       elems;
                                        T                              min_elem;
                                        T                              max_elem;
                                        T                              mid_elem;
                                        std::unique_ptr<histogram_bin> left, right;
                                        std::size_t                    size;

                                        std::size_t increment(const T& elem, const std::size_t count = 1)
                                        {
                                                this->size += count;
                                                if(this->elems.find(elem) == this->elems.end())
                                                        this->elems[elem] = count;
                                                else
                                                        this->elems[elem] += count;
                                                this->min_elem = std::min(this->min_elem, elem);
                                                this->max_elem = std::max(this->max_elem, elem);
                                                return this->size;
                                        }

                                        std::size_t num_elems()
                                        {
                                                return this->elems.size();
                                        }

                                        void make_inner()
                                        {
                                                this->leaf     = false;
                                                this->mid_elem = (this->min_elem + this->max_elem) / 2;
                                                this->elems.clear();
                                                this->size = 0;
                                        }

                                        void make_leaf()
                                        {
                                                this->leaf     = true;
                                                this->max_elem = std::numeric_limits<T>::lowest();
                                                this->min_elem = std::numeric_limits<T>::max();
                                                this->elems.clear();
                                                this->size = 0;
                                        }

                                        histogram_bin(const histogram_bin&)            = delete;
                                        histogram_bin& operator=(const histogram_bin&) = delete;
                                        histogram_bin()
                                        {
                                                this->make_leaf();
                                                this->left.reset(nullptr);
                                                this->right.reset(nullptr);
                                        }
                        };

                        std::unique_ptr<histogram_bin> root;

                        void __insert(std::unique_ptr<histogram_bin>& node, T elem, std::size_t count = 1)
                        {
                                // Allocate a node if one does not exist yet
                                if(!node)
                                {
                                        node = std::make_unique<typename balancing_histogram::hist<T, N>::histogram_bin>();
                                        node->increment(elem, count);
                                }
                                else
                                {
                                        // Step 1: Reach the correct node to insert the element into
                                        if(!node->leaf)
                                        {
                                                if(elem <= node->mid_elem)
                                                        this->__insert(node->left, elem, count);
                                                else
                                                        this->__insert(node->right, elem, count);
                                                return;
                                        }

                                        // Step 2: Insert the element
                                        std::size_t size = node->increment(elem, count);

                                        // Step 3: Check if we have exceeded the maximum node size N. If yes, split it
                                        if(size > N && node->num_elems() > 1) //  We exceed the size only if the total count exceeds N and there are more than 1 bins
                                        {
                                                // Get the split point
                                                T min_elem = node->min_elem;
                                                T max_elem = node->max_elem;
                                                T mid_elem = (max_elem + min_elem) / 2;

                                                // Assign a left child
                                                node->left = std::make_unique<typename balancing_histogram::hist<T, N>::histogram_bin>();

                                                // Assign a right child
                                                node->right = std::make_unique<typename balancing_histogram::hist<T, N>::histogram_bin>();

                                                // Transfer the elements of node to the children
                                                for(std::pair<const T, std::size_t>& p: node->elems)
                                                {
                                                        if(p.first <= mid_elem)
                                                                node->left->increment(p.first, p.second);
                                                        else
                                                                node->right->increment(p.first, p.second);
                                                }

                                                // Set up the current node correctly
                                                node->make_inner();
                                        }
                                }
                        }

                        std::vector<T> __inorder_traversal_cuts(std::unique_ptr<histogram_bin>& node)
                        {
                                std::vector<T> cuts;

                                // Step 1: Check if we are at a leaf node. If yes, add the boundaries to cuts and return
                                if(node->leaf)
                                {
                                        cuts.push_back(node->min_elem);
                                        return cuts;
                                }

                                // Step 2: If we are not a leaf node, perform in-order traversal and populate cuts
                                std::vector<T> left_cuts  = this->__inorder_traversal_cuts(node->left);
                                std::vector<T> right_cuts = this->__inorder_traversal_cuts(node->right);

                                cuts = left_cuts;
                                cuts.insert(cuts.end(), right_cuts.begin(), right_cuts.end()); // Append the right cuts
                                return cuts;
                        }

                        void __print(std::unique_ptr<histogram_bin>& node)
                        {
                                if(!node->leaf)
                                {
                                        std::cout << "Inner Node" << std::endl;
                                        std::cout << "{" << std::endl;
                                        this->__print(node->left);
                                        this->__print(node->right);
                                        std::cout << "}" << std::endl;
                                }
                                else
                                {
                                        std::cout << node->min_elem << "->" << node->max_elem << " [ ";
                                        for(std::pair<const T, std::size_t>& p: node->elems) std::cout << p.first << "(" << p.second << ") ";
                                        std::cout << "]" << std::endl;
                                }
                        }

                public:
                        void increment(T elem, std::size_t count)
                        {
                                this->__insert(this->root, elem, count);
                        }

                        void insert(T elem)
                        {
                                this->__insert(this->root, elem);
                        }

                        std::vector<T> get_bin_boundaries()
                        {
                                return this->__inorder_traversal_cuts(this->root);
                        }

                        void print()
                        {
                                this->__print(this->root);
                        }
        };
} // namespace balancing_histogram
