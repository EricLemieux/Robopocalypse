#ifndef ANIMATION_MATH_H
#define ANIMATION_MATH_H

#define PI 3.14159265359

//LERP
template<typename lerpClass>
inline lerpClass LERP(const lerpClass p0, const lerpClass p1, const float t){
	//blending
	return ((1-t)*p0) + ((t)*p1);
	//faster
	//return ((p1-p0)*t +p0);
}

//NLERP
template <typename nlerpClass>
inline nlerpClass NLERP(const nlerpClass P0, const nlerpClass P1, const float t)
{
	return nlerpClass(glm::normalize(LERP(P0,P1,t)));
}

//CATMULL-ROM INTERPOLATION
template <typename catmullromClass>
inline catmullromClass CatmullRom(const catmullromClass p_i_1, 
      const catmullromClass p_i, 
      const catmullromClass p_i1, 
      const catmullromClass p_i2,
      const float t)
{
 catmullromClass result = 0.5f *
  ( 2*p_i + 
  t*(-p_i_1 + p_i1) +
  t*t*(2*p_i_1 - 5*p_i + 4*p_i1 - p_i2) +
  t*t*t*(-p_i_1 + 3*p_i - 3*p_i1 + p_i2) );

 return result;
}

//Quaternion SLERP
inline glm::quat quatSLERP(glm::quat q0, glm::quat q1, float t)
{
	//glm::vec3 q0v = glm::vec3(q0.x, q0.y, q0.z);
	//glm::vec3 q1v = glm::vec3(q1.x, q1.y, q1.z);

	//float top = (glm::dot(q0, q1));
	//float bot = (glm::dot(glm::l2Norm(q0v),glm::l2Norm(q1v)));

	float q0l2norm = sqrt(q0.w*q0.w + q0.x*q0.x + q0.y*q0.y + q0.z*q0.z);
	float q1l2norm = sqrt(q1.w*q1.w + q1.x*q1.x + q1.y*q1.y + q1.z*q1.z);

	q0 = q0/q0l2norm;
	q1 = q1/q1l2norm;

	float angle = acos(glm::dot(q0, q1)	/	glm::dot(q0l2norm,q1l2norm));
	//angle *= PI/180;	//Convert to degrees

	glm::quat newQuat;

	newQuat = (sin((1-t) * angle) / sin(angle))*q0 + (sin(t * angle) / sin(angle))*q1;

	return newQuat;
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