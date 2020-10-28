#if !defined(__CONTACT_H__)
#define __CONTACT_H__

#include <string>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;

class Contact
{
private:
    int _id;
    std::string _name;
    std::string _phone;

public:
    Contact(int id, std::string name, std::string phone)
        : _id{id}, _name{name}, _phone{phone}
    {
    }
    Contact() = default;
    ~Contact()
    {
    }

    int getId() const { return _id; }
    std::string getName() const { return _name; }
    std::string getPhone() const { return _phone; }

    std::string toJson()
    {
        pt::ptree root;

        root.put("id", _id);
        root.put("name", _name);
        root.put("phone", _phone);

        std::ostringstream os;
        pt::write_json(os, root);
        return os.str();
    }

    void fromJson(std::string jsonString)
    {
        std::stringstream ss;

        ss << jsonString << std::endl;

        pt::ptree root;

        pt::read_json(ss, root);

        _id = root.get<int>("id", 0);
        _name = root.get<std::string>("name", NULL);
        _phone = root.get<std::string>("phone", NULL);
    }
};

#endif // __CONTACT_H__
