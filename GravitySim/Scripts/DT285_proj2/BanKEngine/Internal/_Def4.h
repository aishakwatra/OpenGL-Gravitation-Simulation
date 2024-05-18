#include "_Def3.h"
//BanKEngine Default Components


class GameObj;
class Renderer;

class BanKBehavior;
vector<BanKBehavior*> sBanKBehavior;
class BanKBehavior {

public:
	string SerialID = "Unknown";
	GameObj* GameObject;
	BanKBehavior() {
		//cout << "\tBanKBehavior";
		sBanKBehavior.push_back(this);
	}
	//virtual void Init() {}
	virtual void Start() {}
	virtual void Update() {}

};


class Transform;
vector<Transform*> sTransforms;//Dont Auto Update
class Transform : public BanKBehavior {

public:
	Transform() {
		SerialID = "Transform";
		sTransforms.push_back(this);
	}
	Point PositionWLD = Point(0, 0, -2);
	Point PositionOBJ = Point(0, 0, 0);
	Vector Rotation = Vector(0,0,0);
	float Scale = 1;
	//Vector3 Scale = Vector3(1, 1, 1);
	Affine AffineMatrix;

	Point PositionOBJ2WLD() {
		return Point(AffineMatrix[0].w, AffineMatrix[1].w, AffineMatrix[2].w);
	}

	Vector getForwardVector() {
		return Vector(										
			AffineMatrix.row[0].z,		
			AffineMatrix.row[1].z,		
			AffineMatrix.row[2].z		
		);
	}

	void UpdateTransformMatrix()
	{
	}
};


									vector<GameObj*> sGameObjs;
									class GameObj {

									#define MaxComponent 32

									public:

										Transform Transform;
										GameObj() {
											sGameObjs.push_back(this);
										}
										vector<BanKBehavior*> MyComponents;
										template<typename T>
										T* AddComponent(T* comp)
										{
											MyComponents.reserve(MaxComponent);
											comp->GameObject = this;

											MyComponents.push_back(comp);
											return comp;
										}
										template<typename T>
										T* GetComponent(T Class) {
											for (BanKBehavior* component : MyComponents) {
												if (dynamic_cast<T*>(component)) { return dynamic_cast<T*>(component); }
											}
											return nullptr;
										}

										void Start() {
											for (BanKBehavior* Each : MyComponents) {
												Each->Start();
											}
										}
										void Update() {
											for (BanKBehavior* Each : MyComponents) {
												Each->Update();
											}
										}





										static GameObj* Create() {
											return new GameObj();
										}
										static void Destroy(GameObj* Target) {
											auto it = remove(sGameObjs.begin(), sGameObjs.end(), Target);
											if (it != sGameObjs.end()) {
												sGameObjs.erase(it);
												//delete Target;        
											}
										}
									};
									GameObj* Edit_Obj;





	Vector rot_X = Vector(1, 0, 0);
	Vector rot_Y = Vector(0, 1, 0);
	Vector rot_Z = Vector(0, 0, 1);

class Renderer;
vector<Renderer*> sRenderers;
class Renderer : public BanKBehavior {//Seperated AutoUpdate

public:
	Mesh* MyMesh;
	//GLuint Shader = BanKEngine::Rendering::Shader::ShaderProgram;
	Vector Color = Vector(1, 1, 1);

	bool _Fill = true;
	bool _Wireframe = false;

	Renderer() {
		SerialID = "Renderer";
		sRenderers.push_back(this);
	}

	void SetMesh(Mesh& M) { MyMesh = &M; 	}

	void Draw(Camera &cam) {
		//GameObject->Transform.UpdateTransformMatrix();

		float _scale = max(MyMesh->Dimensions().x, max(MyMesh->Dimensions().y, MyMesh->Dimensions().z));
		GameObject->Transform.AffineMatrix =
			Trans(GameObject->Transform.PositionWLD - OriginWLD)
			* Rot(GameObject->Transform.Rotation.x, rot_X)
			* Rot(GameObject->Transform.Rotation.y, rot_Y)
			* Rot(GameObject->Transform.Rotation.z, rot_Z)
			* Trans(OriginWLD - GameObject->Transform.PositionOBJ)
			* Trans(OriginWLD - MyMesh->Center())
			* Scale(2.0f*GameObject->Transform.Scale / _scale)
			;

		
		if (_Fill) { DisplayFaces(*MyMesh, GameObject->Transform.AffineMatrix, cam, Color); }
		if(_Wireframe) { DisplayEdges(*MyMesh, GameObject->Transform.AffineMatrix, cam, Color); }
	}

};
Renderer* Edit_Renderer;







