#include "tw_demo.h"


void TouchableNode::Update()
{
	if(! isChanged)
		return;
	if(m_old_positions.size() > 1)
	{
		//get current state 
		Vector3 new_max_scale(FLT_MIN,FLT_MIN,0);
		Vector3 new_min_scale(FLT_MAX,FLT_MAX,0);
		Vector3 new_center(Vector3::ZERO);
		vector<Vector3>::iterator it;
		//get the center
		for (it = m_positions.begin();it != m_positions.end(); it++)
		{
			new_max_scale.x = max(new_max_scale.x , it->x);
			new_max_scale.y = max(new_max_scale.y , it->y);

			new_min_scale.x = min(new_min_scale.x , it->x);
			new_min_scale.y = min(new_min_scale.y , it->y);

			new_center += *it;
		}
		new_center /= (float)m_positions.size();

		//get old state 
		Vector3 old_max_scale(FLT_MIN,FLT_MIN,0);
		Vector3 old_min_scale(FLT_MAX,FLT_MAX,0);
		Vector3 old_center(Vector3::ZERO);
		//get the center
		for (it = m_old_positions.begin();it != m_old_positions.end(); it++)
		{
			old_max_scale.x = max(old_max_scale.x , it->x);
			old_max_scale.y = max(old_max_scale.y , it->y);

			old_min_scale.x = min(old_min_scale.x , it->x);
			old_min_scale.y = min(old_min_scale.y , it->y);

			old_center += *it;
		}
		old_center /= (float)m_old_positions.size();

		//compare between both states

		//----------translate
		m_SceneNode->translate(new_center - old_center);

		//----------scale
		//compare state scales
		Vector3 new_scale = new_max_scale - new_min_scale;
		Vector3 old_scale = old_max_scale - old_min_scale;
		float sx = abs(new_scale.x / old_scale.x);
		float sy = abs(new_scale.y / old_scale.y);
		//add it to the original scalling of the object
		Vector3 current_scale = m_SceneNode->getScale();
		current_scale.x *= sx;
		current_scale.y *= sy;
		//keep minimum size allowed .. its a picture after all
		current_scale.x = max(0.00001f, current_scale.x );
		current_scale.y = max(0.00001f, current_scale.y );
		//commit
		m_SceneNode->setScale(current_scale);

		//-------Rotate
		//compare rotate states
		Vector3 new_dir = m_positions[0] - new_center;
		Vector3 old_dir = m_old_positions[0] - old_center;
		Quaternion q1 = Vector3::UNIT_X.getRotationTo(old_dir);
		Quaternion q2 = Vector3::UNIT_X.getRotationTo(new_dir);
		q2 = q2 - q1;
		//add to original rotation
		Quaternion q = m_SceneNode->getOrientation();
		q = q + q2;
		//commit
		m_SceneNode->setOrientation(q);

		tw_demoApp::getSingleton().mDebugText = StringConverter::toString(old_center) + " \r\n" + StringConverter::toString(new_center);
	}

	//update positions
	m_old_positions.clear();
	if(m_positions.size() > 0)
	{
		insert_iterator<vector<Vector3>> insIt(m_old_positions,m_old_positions.begin());
		copy(m_positions.begin(),m_positions.end(),insIt);
	}

}


TouchableNode::TouchableNode( SceneNode* sn)
{
	m_SceneNode = sn;
	isChanged = false;
}
int TouchableNode::AddControlPoint(Vector3& pos)
{
	isChanged = true;
	m_positions.push_back(pos);
	return m_positions.size()-1;
}

void TouchableNode::RemoveControlPoint( int at )
{
	if(m_positions.size() > at )
		m_positions.erase( m_positions.begin() + at );
	if(m_old_positions.size() > at)
		m_old_positions.erase( m_old_positions.begin() + at );

}

void TouchableNode::MoveControlPoint( int at ,Vector3& new_pos)
{
	isChanged = true;
	if(m_positions.size() > at)
		m_positions[at] = new_pos;
}