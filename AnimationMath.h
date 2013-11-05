#ifndef ANIMATION_MATH_H
#define ANIMATION_MATH_H

template<typename lerpClass>
inline lerpClass LERP(const lerpClass p0, const lerpClass p1, const float t){
	//blending
	return ((1-t)*p0) + ((t)*p1);
	//faster
	return ((p1-p0)*t +p0);
}

//CATMULL-ROM INTERPOLATION
template <typename catmullromClass>
inline float CatmullRom(const catmullromClass p_i_1, 
      const catmullromClass p_i, 
      const catmullromClass p_i1, 
      const catmullromClass p_i2,
      const catmullromClass t)
{
 catmullromClass result = 0.5f *
  ( 2*p_1 + 
  t*(-p_i_1 + p_i1) +
  t*t*(2*p_i_1 - 5*p_i + 4*p_i1 - p_i2) +
  t*t*t*(-p_1_1 + 3*p_i - 3*p_i1 + p_i2) );

 return result;
}

template<typename bezierClass>
inline bezierClass Bezier(const bezierClass p0, 
						  const float t){
	return p0;
}

template<typename bezierClass>
inline bezierClass Bezier(const bezierClass p0, 
						  const bezierClass p1, 
						  const float t){
	return LERP<bezierClass>(Bezier(p0,t),Bezier(p1,t),t);
}

template<typename bezierClass>
inline bezierClass Bezier(const bezierClass p0, 
						  const bezierClass p1,
						  const bezierClass p2,
						  const float t){
	return LERP<bezierClass>(Bezier(p0,p1,t),Bezier(p1,p2,t),t);
}

template<typename bezierClass>
inline bezierClass Bezier(const bezierClass p0, 
						  const bezierClass p1,
						  const bezierClass p2,
						  const bezierClass p3,
						  const float t){
	return LERP<bezierClass>(Bezier(p0,p1,p2,t),Bezier(p1,p2,p3,t),t);
}

template<typename bezierClass>
inline bezierClass Bezier(const bezierClass p[],
						  const unsigned int degree,// this is number of points -1, line segments
						  const float t){
	if(degree == 0)
		return *p;//ie p[0]
	return LERP(Bezier(p, degree-1,t), Bezier( p+1,degree-1,t),t);
}

#endif