

class Integrations{
	public:
		Integrations();
		virtual ~Integrations();
		
		float ForwardEuler(float u_n, float vComp, float stepSize);
		
		float RungeKutta(float u_n, float stepSize,
						 float vComp, float vCompDelta);
};