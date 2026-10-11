// Original: d:\build\custom\temp\code\client\wangreal\source\wbone_physics.cpp
#include <math.h>

#include "../include/wbone.h"

// wbone_physics.cpp:16
void WBone::SetPhysicsModel(int type) {
	int cnt = 0;
	int offset = 0;
	w_mesh* mesh;

	for (mesh = m_mesh; mesh != 0; mesh = mesh->next) {
		cnt += mesh->vtxNum;
		++offset;
	}

	m_flag.Enable(PHYSICSMODEL);
	m_physic = new w_bone_physic_link;
	m_physic->vtxnum = cnt;
	m_physic->pos = new WVector[cnt];
	m_physic->vel = new WVector[cnt];
	m_physic->linkoffset = new int[cnt];
	m_physic->list = new WVector*[offset];

	mesh = m_mesh;
	offset = 0;
	cnt = 0;
	int meshIndex = 0;
	for (; mesh != 0; mesh = mesh->next, ++meshIndex) {
		for (int i = mesh->rigidNum; i < mesh->vtxNum; ++i)
			cnt += FindLink(i, mesh, 0, offset);
		m_physic->list[meshIndex] = m_physic->pos + offset;
		offset += mesh->vtxNum;
	}

	m_physic->link = new w_bone_physic_link::w_bone_physic_link_element[cnt];
	mesh = m_mesh;
	offset = 0;
	cnt = 0;
	for (; mesh != 0; mesh = mesh->next) {
		int i;

		for (i = 0; i < mesh->rigidNum; ++i)
			m_physic->linkoffset[offset + i] = -1;
		for (i = mesh->rigidNum; i < mesh->vtxNum; ++i) {
			m_physic->linkoffset[offset + i] = cnt;
			cnt += FindLink(i, mesh, m_physic->link + cnt, offset);
		}
		offset += mesh->vtxNum;
	}
	ResetPhysicsModel();
}

// wbone_physics.cpp:63
void WBone::ReleasePhysicsModel() {
	if (m_physic != 0) {
		delete m_physic->link;
		delete m_physic->linkoffset;
		delete m_physic->pos;
		delete m_physic;
	}
}

// wbone_physics.cpp:74
void WBone::UpdatePhysicsModel(float delta, const WVector& accel) {
	WVector pos;
	WVector gap;
	WVector total;
	float ext;
	float magnitude;
	int i;

	for (i = 0; i < m_physic->vtxnum; i++) {
		if (m_physic->linkoffset[i] >= 0)
			m_physic->vel[i] += accel * delta;
	}

	for (i = 0; i < m_physic->vtxnum; i++) {
		if (m_physic->linkoffset[i] >= 0) {
			w_bone_physic_link::w_bone_physic_link_element* link = &m_physic->link[m_physic->linkoffset[i]];
			pos = m_physic->pos[i] + m_physic->vel[i] * delta;
			total = WVector::ZERO;

			for (int j = 0; link[j].offset >= 0; j++) {
				gap = m_physic->pos[link[j].offset] - pos;
				magnitude = gap.Magnitude();
				ext = magnitude - link[j].length;
				magnitude = ext / magnitude;
				total += gap * magnitude;
			}

			m_physic->vel[i] += total;
		}
	}

	for (i = 0; i < m_physic->vtxnum; i++)
		m_physic->pos[i] += m_physic->vel[i] * delta;
}

// wbone_physics.cpp:111
int WBone::FindLink(int body, w_mesh* m_mesh, w_bone_physic_link::w_bone_physic_link_element* out, int offset) {
	int temp[16];
	int count = 0;
	int i;
	int j;
	int k;
	int n;

	for (i = 0; i < m_mesh->indexNum; i += 3) {
		for (j = 0; j < 3; j++) {
			if (m_mesh->indexList[i + j] == body) {
				for (k = 0; k < 3; k++) {
					if (j != k) {
						int index = m_mesh->indexList[i + k];

						for (n = 0; n < count && temp[n] != index; n++);
						if (n == count)
							temp[count++] = index;
					}
				}
			}
		}
	}

	if (out != 0) {
		for (n = 0; n < count; n++) {
			out[n].offset = temp[n] + offset;
			out[n].length = (m_mesh->vecList[body] - m_mesh->vecList[temp[n]]).Magnitude();
		}
		out[n].offset = -1;
	}

	return count + 1;
}

// wbone_physics.cpp:152
void WBone::ResetPhysicsModel() {
	int j = 0;

	for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
		for (int i = 0; i < mesh->vtxNum; ++i, ++j) {
			m_physic->pos[j] = mesh->vecList[i] * m_matrix;
			m_physic->vel[j] = WVector::ZERO;
		}
	}
}
