#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
class Element : public std::enable_shared_from_this<Element> {
   public:
    std::string name;
    std::unordered_map<std::string, std::string> attributes;
    std::string innerText;
    std::vector<std::shared_ptr<Element>> children;
    std::weak_ptr<Element> parent;

    Element(const std::string& name) : name(name) {}

    void AddChild(const std::shared_ptr<Element>& child) {
        child->parent = shared_from_this();
        children.push_back(child);
    }

    bool HasAttribute(const std::string& key) const {
        return attributes.find(key) != attributes.end();
    }

    std::string GetAttribute(const std::string& key) const {
        auto iter = attributes.find(key);
        return iter != attributes.end() ? iter->second : "";
    }
};