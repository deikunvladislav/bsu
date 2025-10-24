#pragma once
#include <memory>
#include <vector>

namespace app::list {

    template <typename T>
    struct Node {
        T value;
        std::unique_ptr<Node<T>> next;
        explicit Node(T v) : value(std::move(v)), next(nullptr) {}
    };

    template <typename T>
    using NodePtr = std::unique_ptr<Node<T>>;

    template <typename T>
    NodePtr<T> from_vector(const std::vector<T>& v) {
        NodePtr<T> head = nullptr; Node<T>* tail = nullptr;
        for (auto& x : v) {
            auto node = std::make_unique<Node<T>>(x);
            if (!head) { tail = node.get(); head = std::move(node); }
            else { tail->next = std::move(node); tail = tail->next.get(); }
        }
        return head;
    }

    template <typename T>
    std::vector<T> to_vector(const NodePtr<T>& head) {
        std::vector<T> out;
        for (auto* p = head.get(); p; p = p->next.get()) out.push_back(p->value);
        return out;
    }

    template <typename T>
    struct RevResult {
        NodePtr<T> head;
        Node<T>* tail; 
    };

    template <typename T>
    RevResult<T> reverse_impl(NodePtr<T> head) {
        if (!head) return { nullptr, nullptr };
        if (!head->next) {
            Node<T>* raw = head.get();
            return { std::move(head), raw };
        }
        auto next = std::move(head->next);
        auto rr = reverse_impl<T>(std::move(next));
        rr.tail->next = std::move(head);
        rr.tail = rr.tail->next.get();
        rr.tail->next = nullptr;
        return rr;
    }

    template <typename T>
    NodePtr<T> reverse_recursive(NodePtr<T> head) {
        return reverse_impl<T>(std::move(head)).head;
    }
}