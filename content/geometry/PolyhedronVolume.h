/**
 * Author: ?
 * Date: ?
 * Source: tinyKACTL
 * Description: Magic formula for the volume of a polyhedron. Faces should point outwards.
 * Status: untested
 */
#pragma once

template <class V, class L>
double signed_poly_volume(const V& p, const L& trilist) {
  double v = 0;
  trav(i, trilist) v += p[i.a].cross(p[i.b]).dot(p[i.c]);
  return v / 6;
}