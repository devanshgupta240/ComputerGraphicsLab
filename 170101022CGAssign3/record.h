#ifndef RECORD_H
#define RECORD_H
#include "threesphere.h"
#include "singlerec.h"
#include <memory>
#include <vector>
class record : public singlerec {
    public:
        record() {}
        record(shared_ptr<singlerec> object) { add(object); }

        void clear() { objects.clear(); }
        void add(shared_ptr<singlerec> object) { objects.push_back(object); }

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        std::vector<shared_ptr<singlerec>> objects;
};
bool record::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;
    auto hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}
#endif
