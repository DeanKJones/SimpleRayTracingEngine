#include "Hittable.h"
#include "Common\aabb.h"
#include "Random.h"

class bvhNode : public Hittable 
{
public:
    bvhNode() {}
    bvhNode(Hittable **l, int n, float time0, float time1);

    virtual bool hit(const Ray& r, float tmin, float tmax, hitRecord& rec) const;
    virtual bool boundingBox(float t0, float t1, aabb& box) const;

    Hittable *left;
    Hittable *right;
    aabb box;
};

bool bvhNode::boundingBox(float t0, float t1, aabb& b) const {
    b = box;
    return true;
}

bool bvhNode::hit(const Ray& r, float t_min, float t_max, hitRecord& rec) const {
    if (box.hit(r, t_min, t_max)) {
        hitRecord left_rec, right_rec;

        bool hit_left = left->hit(r, t_min, t_max, rec);
        bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

        if (hit_left && hit_right) {
            if (left_rec.t < right_rec.t)
                rec = left_rec;
            else
                rec = right_rec;
            return true;
        }
        else if (hit_left) {
            rec = left_rec;
            return true;
        }
        else if (hit_right) {
            rec = right_rec;
            return true;
        }
        else
            return false;
    }
    else return false;
}

int box_x_compare (const void * a, const void * b) {
    aabb box_left, box_right;
    Hittable *ah = *(Hittable**)a;
    Hittable *bh = *(Hittable**)b;
    if (!ah->boundingBox(0,0, box_left) || !bh->boundingBox(0,0, box_right))
        std::cerr << "no bounding box in bvh_node constructor\n";
    if (box_left.min().x() - box_right.min().x() < 0.0)
        return -1;
    else
        return 1;
}

int box_y_compare (const void * a, const void * b) {
    aabb box_left, box_right;
    Hittable *ah = *(Hittable**)a;
    Hittable *bh = *(Hittable**)b;
    if (!ah->boundingBox(0,0, box_left) || !bh->boundingBox(0,0, box_right))
        std::cerr << "no bounding box in bvh_node constructor\n";
    if (box_left.min().x() - box_right.min().x() < 0.0)
        return -1;
    else
        return 1;
}

int box_z_compare (const void * a, const void * b) {
    aabb box_left, box_right;
    Hittable *ah = *(Hittable**)a;
    Hittable *bh = *(Hittable**)b;
    if (!ah->boundingBox(0,0, box_left) || !bh->boundingBox(0,0, box_right))
        std::cerr << "no bounding box in bvh_node constructor\n";
    if (box_left.min().x() - box_right.min().x() < 0.0)
        return -1;
    else
        return 1;
}

bvhNode::bvhNode(Hittable **l, int n, float time0, float time1) {
    int axis = int(3 * randomDouble());

    if (axis == 0)
        qsort(l, n, sizeof(Hittable *), box_x_compare);
    else if (axis == 1)
        qsort(l, n, sizeof(Hittable *), box_y_compare);
    else
        qsort(l, n, sizeof(Hittable *), box_z_compare);

    if (n == 1) {
        left = right = l[0];
    }
    else if (n == 2) {
        left = l[0];
        right = l[1];
    }
    else {
        left = new bvhNode(l, n/2, time0, time1);
        right = new bvhNode(l + n/2, n - n/2, time0, time1);
    }
    aabb box_left, box_right;
    if (!left->boundingBox(time0, time1, box_left) ||
        !right->boundingBox(time0, time1, box_right)) 
    {
        std::cerr << "no bounding box in bvh_node constructor\n";
    }
    box = surroundingBox(box_left, box_right);
}