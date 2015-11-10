#include "xmlobjects.hpp"
#include "md5.hpp"
#include "litesql.hpp"
#include "logger.hpp"

#include <algorithm>
#include <map>

namespace xml {

const char* Database::TAG="database";
const char* Object::TAG="object";
const char* Field::TAG="field";
const char* Relation::TAG="relation";
const char* Relate::TAG="relate";
const char* Value::TAG="value";
const char* IndexField::TAG="indexfield";
const char* Index::TAG="index";
const char* Param::TAG="param";
const char* Method::TAG="method";

ObjectPtr Object::DEFAULT_BASE(new Object("litesql::Persistent",""));

Field::Ptr Object::ID_FIELD(new Field("id", A_field_type_integer, "", A_field_indexed_true, A_field_unique_true));
Field::Ptr Object::TYPE_FIELD(new Field("type", A_field_type_string, "", A_field_indexed_false, A_field_unique_false));


string validID(const string& s, const string& type="field") {
  string result="";

  if (s.empty())
    result = "empty identifier";
  else if (toupper(s[0]) == s[0] && type == "field")
    result =  "does not begin with lower case letter";
  else if (!isalpha(s[0]))
    result = "first character is not alphabet";
  else if (s[s.size()-1] == '_')
        result = "ends with an underscore";
  else
  {
    for (size_t i = 1; i < s.size(); i++)
    {
      if (!isalnum(s[i]) && !(s[i] != '_'))
      {
        result = "illegal character : ";
        result += s[i];
        break;
      }
    }
  }
  return result;
}

string makeDBName(const string& s) {
  if(true) {
    //ORACLE allows only 30 chars and leading alphabet is a must
    std::string name(s);
    if (s.size() > 30) {
      name = "O" + md5HexDigest(s).substr(0,29);
    }

    // predefine index table
    // for sakura project
    std::map<std::string, std::string> predefined_table;
    predefined_table["O8d603742f857bf0bc93ab6ffe7aae"] = "O4733217f174c3c50f5ac38ffec5cf";
    predefined_table["O7a73a482339780f9811c3724533ca"] = "O15bd989303e893b42d1295a04bee5";
    predefined_table["O12f5e619588c5abfc527ac57ac7b4"] = "O348d6370565e70abb489d3585136c";
    predefined_table["Ob5e7f1cc5157fca21196b7ccf74fc"] = "O12466c812aea4f27e5553e708b4ed";
    predefined_table["Od225122f4696aeefdb103227fd38e"] = "Ofef4ea5fa511675232cca251825f7";
    predefined_table["O532c2b676f7db9363b53b45afcb4a"] = "O6a80e4247659f75f12a13d092697b";
    predefined_table["O81e7a5176b8058cd363772ac1c971"] = "Oe17d61b057caa623466136e180182";
    predefined_table["O2791f0bf05a8ec6721336764f016d"] = "Odd9b508c1797b3bebd94c0b90acc3";
    predefined_table["Obd8ff7d74f0f3aea251169ea3e199"] = "Oc1b8bdb55c7f0826afae5e4fd1c2d";
    predefined_table["O1b1e47caede08c3e446e6ade7e83c"] = "Ocd8b26538c3738515da04bfff27cf";
    predefined_table["O5321dbef39a4dcff5b533a2ed1108"] = "O005de571e9174d0b2fb747f8018ed";
    predefined_table["O5f3416153b1635908ef538184e2bd"] = "O2898bc15b3782ee1527bdf8aa7b5a";
    predefined_table["O71079bae530202afdb66c860c7893"] = "O662c270704ced9358679661f7e1d1";
    predefined_table["O590b06232a49006f039b2aa13ac4a"] = "O7ed5da37f9ade3873c2a0351bb069";
    predefined_table["O55d01e19a150cbc9f49b386accde5"] = "O5ad90c13bcd7ec8651e51d3179ccd";
    predefined_table["Of020665491f0ea2977fd923a4ab66"] = "O0608b2a99a19399d64b93c541eafb";
    predefined_table["O757d76166f1d41e68c5eaa7830919"] = "O6add182f8bf282d9ea42808f440e0";
    predefined_table["Ob04abb75cd8015b2a67910f81c352"] = "O98f8662fcb3f00a915e7fae54170f";
    predefined_table["Of851dbe7a77bde6031c47710d9609"] = "O49d773c941bdc8ccb5f3b875f8024";
    predefined_table["O9ba708d7277ae79c49b1c429fddfb"] = "Ofd7580780baab3d1c5b7b923b1a32";
    predefined_table["O8ca82fd545b9042b8300e0d7ed509"] = "O3bd526a1a889bc48a828d80515057";
    predefined_table["O0f4328c27d8a4de8a1a68696ccc56"] = "Ofa039bc6e7405eecb5142eb70c982";
    predefined_table["O6a200f089793972759f1dc8a7fe3b"] = "O296579cfa263673c73ed4df6bd85c";
    predefined_table["O8d31c33910a7c3911e62e3a2ee37d"] = "O7b7f94a0422257051ba345b1c3674";
    predefined_table["O92b900443d686ca224f95dfd3bdc9"] = "Oea47dd317a94d8973d012c414ad9a";
    predefined_table["O3addde37c8ed44cbdb3d2fb462e40"] = "O8e5c74673bb0c34fd52bcdc1f7b85";
    predefined_table["O549e4e2b96320ec960f4e6cd700b2"] = "O0259cc6d11c07a9d74119fd67f36b";
    predefined_table["O012d168f61855296724777fa390a5"] = "O9635aecf3961f855c6e110ffff9a9";


    std::map<std::string, std::string>::iterator found = predefined_table.find(name);
    if(found == predefined_table.end()) {
      return name;
    } else {
      return found->second;
    }
  }
  else
  {
    if (s.size() > 31)
      return "_" + md5HexDigest(s);
    return s;
  }
}

static void sanityCheck(DatabasePtr& db,
                        const ObjectSequence& objects,
                        const Relation::sequence& relations) {
    using namespace litesql;
    map<string, bool> usedID;
    map<string, bool> objectName;
    string err;
    if (!(err = validID(db->name,"class")).empty())
        throw Except("invalid id: database.name : " + db->name);
    for (size_t i = 0; i < objects.size(); i++) {
        Object& o = *objects[i];
        if (!(err = validID(o.name, "class")).empty())
            throw Except("invalid id: object.name : " + o.name);
        if (usedID.find(o.name) != usedID.end())
            throw Except("duplicate id: object.name : " + o.name);
        usedID[o.name] = true;
        objectName[o.name] = true;
        map<string, bool> usedField;
        usedField.clear();
        for (size_t i2 = 0; i2 < o.fields.size(); i2++) {
            Field& f = *o.fields[i2];
            if (!(err = validID(f.name)).empty())
              throw Except("invalid id: object.field.name : " + o.name + "." + f.name + " cause:" + err );
            if (usedField.find(f.name) != usedField.end())
                throw Except("duplicate id: object.field.name : " + o.name + "." + f.name);
            usedField[f.name] = true;
        }
    }
    for (size_t i = 0; i < relations.size(); i++) {
        Relation& r = *relations[i];
        string name = r.getName();
        if (!(err = validID(name,"class")).empty())
            throw Except("invalid id: relation.name : " + name + "cause:" + err );
        if (usedID.find(name) != usedID.end())
            throw Except("duplicate id: relation.name : " + name);
        usedID[name] = true;
        map<string, bool> usedField;
        usedField.clear();

        bool defaults = false;

        for (size_t i2 = 0; i2 < r.fields.size(); i2++) {
            Field& f = *r.fields[i2];
            if (!(err = validID(f.name)).empty())
                throw Except("invalid id: relation.field.name : " + name + "." + f.name + "cause:" + err );
            if (usedField.find(f.name) != usedField.end())
                throw Except("duplicate id: relation.field.name : " + name + "." + f.name);
            usedField[f.name] = true;
            if (f.default_.size() > 0)
                defaults = true;
            else if (defaults)
                throw Except("no default-value after field with default value : " + name + "." + f.name);

        }
        usedField.clear();
        bool limits = false;
        bool uniques = false;
        for (size_t i2 = 0; i2 < r.related.size(); i2++) {
            Relate& rel = *r.related[i2];
            if (!(err = validID(rel.handle)).empty() && !rel.handle.empty())
                throw Except("invalid id: relation.relate.handle : " + name + "." + rel.handle + "cause:" + err );
            if (usedField.find(rel.handle) != usedField.end())
                throw Except("duplicate id: relation.relate.handle : " + name + "." + rel.handle);
            if (objectName.find(rel.objectName) == objectName.end())
                throw Except("unknown object: relation.relate.name : " + name + "." + rel.objectName);
            if (!rel.handle.empty())
                usedField[rel.handle] = true;
            if (rel.isUnique())
                uniques = true;
            if (rel.hasLimit())
                limits = true;
            if (uniques && limits)
                throw Except("both 'unique' and 'limit' attributes used in relation " + name);
        }
        if (r.related.size() != 2 && limits)
            throw Except("'limit' attribute used in relation of " + toString(r.related.size())
                         + " object(s) " + name);

    }
}

static void initSchema(DatabasePtr& db,
                ObjectSequence& objects,
                Relation::sequence& relations) {
    for (size_t i = 0; i < objects.size(); i++) {
        Object& o = *objects[i];
        map<string, Database::DBField::Ptr> fldMap;
        Database::Table::Ptr tbl(new Database::Table);
        tbl->name = o.getTable();
        db->tables.push_back(tbl);

        if (!o.parentObject.get()) {
          Database::Sequence::Ptr seq(new Database::Sequence);
          seq->name = o.getSequence();
          seq->table = o.getTable();
          db->sequences.push_back(seq);
        }  else {
          Database::DBField::Ptr id(new Database::DBField);
          id->name("id");
          id->field->type = A_field_type_integer;
          id->primaryKey = true;
          tbl->fields.push_back(id);
        }

        for (size_t i2 = 0; i2 < o.fields.size(); i2++) {
            Field::Ptr f = o.fields[i2];
            Database::DBField::Ptr fld(new Database::DBField);
            fld->name(f->name);
            fldMap[f->name] = fld;
            fld->field->type = f->type;
            fld->primaryKey = (f->name == "id");
            fld->field->unique =  (f->isUnique())
                                 ? A_field_unique_true
                                 : A_field_unique_false;

            fld->field = o.fields[i2];
            tbl->fields.push_back(fld);

            if (f->isIndexed()) {
              Database::DBIndex::Ptr idx(new Database::DBIndex);
                idx->name = makeDBName(tbl->name + fld->name() + "idx");
                idx->table = tbl->name;
                idx->fields.push_back(fld);
                db->indices.push_back(idx);
            }

        }
        for (size_t i2 = 0; i2 < o.indices.size(); i2++) {
            const Index& idx = *o.indices[i2];

            litesql::Split fldNames;
            Database::DBIndex::Ptr index(new Database::DBIndex);
            for (size_t i3 = 0; i3 < idx.fields.size(); i3++) {
                if (fldMap.find(idx.fields[i3].name) == fldMap.end())
                    throw litesql::Except("Indexfield " + o.name + "." + idx.fields[i3].name + " is invalid.");
                index->fields.push_back(fldMap[idx.fields[i3].name]);
                fldNames.push_back(idx.fields[i3].name);
            }

            index->name = makeDBName(tbl->name + "_" + fldNames.join("_") + "_idx");
            index->table = tbl->name;

            string unique = "";
            if (idx.isUnique())
                index->unique = true;
            db->indices.push_back(index);
        }
    }
    for (size_t i = 0; i < relations.size(); i++) {
        Relation& r = *relations[i];

        Database::Table::Ptr tbl(new Database::Table);
        tbl->name = r.getTable();

        db->tables.push_back(tbl);

        Database::DBField::sequence objFields;
        map<string, Database::DBField::Ptr> fldMap;
        for (size_t i2 = 0; i2 < r.related.size(); i2++) {
            const xml::Relate& relate = *r.related[i2];
            AT_field_unique unique = A_field_unique_false;
            if (relate.isUnique())
            {
                unique = A_field_unique_true;
            }
            else if (r.related.size() == 2) {
                if (i2 == 0 && r.related[1]->hasLimit()) {
                    unique = A_field_unique_true;
                }
                if (i2 == 1 && r.related[0]->hasLimit()) {
                    unique = A_field_unique_true;
                }
            }
            Database::DBField::Ptr fld(new Database::DBField);
            fld->name(relate.fieldName);
            fld->field->type = A_field_type_integer;
            fld->field->unique = unique;
            tbl->fields.push_back(fld);
            objFields.push_back(fld);

            Database::DBIndex::Ptr idx( new Database::DBIndex);
            idx->name = makeDBName(tbl->name + fld->name() + "idx");
            idx->table = tbl->name;
            idx->fields.push_back(fld);
            db->indices.push_back(idx);

        }
        for (size_t i2 = 0; i2 < r.fields.size(); i2++) {
            Field& f = *r.fields[i2];
            Database::DBField::Ptr fld(new Database::DBField);
            fld->name(f.name);
            fldMap[f.name] = fld;
            fld->field->type = f.type;
            fld->primaryKey = false;
            fld->field->unique = f.isUnique() ? A_field_unique_true : A_field_unique_false;
            fld->field = r.fields[i2];
            tbl->fields.push_back(fld);

            if (f.isIndexed()) {
              Database::DBIndex::Ptr idx( new Database::DBIndex);
                idx->name = makeDBName(tbl->name + fld->name() + "idx");
                idx->table = tbl->name;
                idx->fields.push_back(fld);
                db->indices.push_back(idx);
            }

        }

        if (r.related.size() > 1) {

          Database::DBIndex::Ptr idx(new Database::DBIndex);
            idx->name = makeDBName(tbl->name + "_all_idx");
            idx->table = tbl->name;
            for (size_t i2 = 0; i2 < objFields.size(); i2++)
                idx->fields.push_back(objFields[i2]);
            db->indices.push_back(idx);
        }
        for (size_t i2 = 0; i2 < r.indices.size(); i2++) {
            const Index& idx = *r.indices[i2];

            litesql::Split fldNames(idx.fields.size());
            Database::DBIndex::Ptr index(new Database::DBIndex);
            for (size_t i3 = 0; i3 < idx.fields.size(); i3++) {
//                Database::DBField* fld = new Database::DBField;
                if (fldMap.find(idx.fields[i3].name) == fldMap.end())
                    throw litesql::Except("Indexfield " + r.name + "." + idx.fields[i3].name + " is invalid.");
                index->fields.push_back(fldMap[idx.fields[i3].name]);
                fldNames.push_back(idx.fields[i3].name);
            }

            index->name = makeDBName(tbl->name + "_" + fldNames.join("_") + "_idx");
            string unique = "";
            if (idx.isUnique())
                index->unique = true;
            db->indices.push_back(index);
        }
    }

}
void init(DatabasePtr& db,
          ObjectSequence& objects,
          Relation::sequence& relations) {
    map<string, ObjectPtr> objMap;
    Logger::report("validating XML file\n");
    sanityCheck(db, objects, relations);
    Logger::report("linking XML - objects\n");
    // make string -> Object mapping

    for (ObjectSequence::const_iterator object = objects.begin();
      object != objects.end();
      ++object)
    {
      objMap[(*object)->name] = (*object);
    }
    // make Object's class hierarchy mapping (parent and children)

    for (size_t i = 0; i < objects.size(); ++i)
      if (objMap.find(objects[i]->inherits) != objMap.end()) {
            objects[i]->parentObject = objMap[objects[i]->inherits];
            objects[i]->parentObject->children.push_back(objects[i]);
      }
    //for (size_t i = 0; i < objects.size(); i++)
    //    if (objects[i]->parentObject)
    //        objects[i]->parentObject->children.push_back(objects[i]);

    // sort objects of relations alphabetically (ascii)

    for (Relation::sequence::const_iterator it_rel=relations.begin();it_rel!=relations.end();++it_rel)
    {
        sort((*it_rel)->related.begin(), (*it_rel)->related.end(),Relate::CompareByObjectName());
    }

    for (Relation::sequence::const_iterator it_rel=relations.begin();it_rel!=relations.end();++it_rel)
    {
        Relation::Ptr rel = *it_rel;
        bool same = rel->sameTypes() > 1;

        for (size_t i2 = 0; i2 < rel->related.size(); i2++) {
          Relate::Ptr& relate = rel->related[i2];
            ObjectPtr& obj = objMap[relate->objectName];
            string num;
            if (same)
                num = toString(i2 + 1);
            relate->fieldTypeName = relate->objectName + num;
            relate->fieldName = relate->objectName + toString(i2 + 1);
            if (obj->relations.find(rel) == obj->relations.end())
              obj->relations[rel] = Relate::sequence();

            // make Object -> Relation mapping

            obj->relations[rel].push_back(relate);
            if (!relate->handle.empty()) {

                // make Object's relation handles

              RelationHandle::Ptr handle (new RelationHandle(relate->handle, rel,relate,obj));
                for (size_t i3 = 0; i3 < rel->related.size(); i3++) {
                    if (i2 != i3) {
                        ObjectPtr& o = objMap[rel->related[i3]->objectName];
                        // make RelationHandle -> (Object,Relation) mapping
                        handle->destObjects.push_back(make_pair(o,rel->related[i3]));
                    }
                }
                obj->handles.push_back(handle);
            }
        }
    }

    initSchema(db, objects, relations);

}
string safe(const char * s) {
    if (s)
        return s;
    return "";
}

string attribute(const string& name, const string& value)
{
  string a;
  a.append(name).append("=").append("\"").append(value).append("\" ");
  return a;
}

string endtag(const string& name)
{
  string a;
  a.append("</").append(name).append(">");
  return a;
}


}
