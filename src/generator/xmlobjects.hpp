#ifndef _litesql_xmlobjects_hpp
#define _litesql_xmlobjects_hpp
#include <string>
#include <vector>
#include <stdexcept>
#include <map>
#include "litesql-gen.hpp"
#include "litesql/split.hpp"
namespace xml {
using namespace std;
using namespace litesql;
class Value {
public:
    string name, value;
    Value(string n, string v) : name(n), value(v) {}
};
class Field {
public:
    string name;
    AT_field_type type;
    string default_;
    AT_field_indexed indexed;
    vector<Value> values;
    Field(string n, AT_field_type t, string d, AT_field_indexed i) 
        : name(n), type(t), default_(d), indexed(i) {}
    void value(const Value& v) {
        values.push_back(v);
    }
    string getQuotedDefaultValue() const {
        if (getCPPType()=="std::string")
            return "\"" + default_ + "\"";
        if (default_.size() == 0)
            return "0";
        return default_;
    }
    string getSQLType() const {
       switch(type) {
           case A_field_type_integer: return "INTEGER";
           case A_field_type_string: return "TEXT";
           case A_field_type_float: return "FLOAT";
           default: return "";
       }
    }
    string getCPPType() const {
       switch(type) {
           case A_field_type_integer: return "int";
           case A_field_type_string: return "std::string";
           case A_field_type_float: return "float";
           default: return "";
       }
    }
};
class Param {
public:
    string name;
    AT_param_type type;
    Param(string n, AT_param_type t) : name(n), type(t) {}
    
};
class Method {
public:
    string name, returnType;
    vector<Param> params;
    Method(string n, string rt) 
        : name(n), returnType(rt) {}
    void param(const Param& p) {
        params.push_back(p);
    }
};
class Relation;
class Relate;
class Object;
class RelationHandle {
public:
    string name;
    Relation * relation;
    Relate * relate;
    Object * object;
    vector< pair<Object*,Relate*> > destObjects;

    RelationHandle(string n, Relation * r, Relate * rel, Object * o) 
        : name(n), relation(r), relate(rel), object(o) {}
};
class Relate {
public:    
    string objectName;
    string fieldTypeName;
    string getMethodName;
    int paramPos;
    AT_relate_limit limit;
    string handle;
    Relate(string on, AT_relate_limit l, string h) 
        : objectName(on), limit(l), handle(h) {}
    bool hasLimit() const {
        return limit == A_relate_limit_one;
    }
};
class Relation {
public:
    string id, name;
    AT_relation_unidir unidir;
    vector<Relate> related;
    vector<Field> fields;
    Relation(string i, string n, AT_relation_unidir ud) 
        : id(i), name(n), unidir(ud) {}
    string getName() const {
        if (name.size() == 0) {
            string result;
            for (size_t i = 0; i < related.size(); i++) 
                result += related[i].objectName;
            return result + "Relation" + id;
        }
        return name;
    }
    bool isUnidir() const {
        return unidir == A_relation_unidir_true;
    }
    int sameTypes() const {
        map<string, int> names;
        int max = 0;
        for (size_t i = 0; i < related.size(); i++) {
            if (names.find(related[i].objectName) == names.end()) 
                names[related[i].objectName] = 0;
            int value =	++names[related[i].objectName];
            if (value > max)
                max = value;
        }
        return max;
    }
    int countTypes(string name) const {
        int res = 0;
        for (size_t i = 0; i < related.size(); i++)
            if (related[i].objectName == name)
                res++;
        return res;
    }
    string getTable() const {
        Split res;
        for (size_t i = 0; i < related.size(); i++)
            res.push_back(related[i].objectName);
        res.push_back(id);
        return res.join("_");
    }
};
class Object {
public:
    string name, inherits;
    vector<Field> fields;
    vector<Method> methods;
    vector<RelationHandle> handles;
    map<Relation*, vector<Relate*> > relations;
    Object * parentObject;
    vector<Object*> children;

    Object(string n, string i) : name(n), inherits(i),
        parentObject(NULL) {
        if (i.size() == 0) {
            inherits = "litesql::Persistent";
            fields.push_back(Field("id", A_field_type_integer, "", 
                         A_field_indexed_true));
            fields.push_back(Field("type", A_field_type_string, "", 
                        A_field_indexed_false));
        }
    }
    int getLastFieldOffset() const {
        if (!parentObject)
            return fields.size();
        else return parentObject->getLastFieldOffset() + fields.size();
    }
    void getAllFields(vector<Field>& flds) const {
        if (parentObject)
            parentObject->getAllFields(flds);
        for (size_t i = 0; i < fields.size(); i++)
            flds.push_back(fields[i]);
    }
    void getChildrenNames(Split & names) const {
        for (size_t i = 0; i < children.size(); i++) {
            names.push_back(children[i]->name);
            children[i]->getChildrenNames(names);
        }
    }
    const Object* getBaseObject() const{
        if (!parentObject)
            return this;
        else
            return parentObject->getBaseObject();
    }
    string getTable() const {
        return name + "_";
    }
};
string safe(const char *s);


}

#endif