

#pragma  once
class TouchableNode
{
public:
	SceneNode* m_SceneNode;
	vector<Vector3> m_positions;
	vector<Vector3> m_old_positions;
	bool isChanged;

	TouchableNode(SceneNode*);
	void Update();
	int AddControlPoint(Vector3 &pos);
	void RemoveControlPoint(int at);
	void MoveControlPoint(int at,Vector3& new_pos);

};