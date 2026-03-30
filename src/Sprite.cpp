//
// Created by aamalh on 3/27/26.
//

#include "Sprite.h"

void Sprite::step(const float dt) {
	m_position += m_velocity * dt;
}

void CircleSprite::step(const float dt) {
	auto tmp_position = m_position + m_velocity * dt;
	if (tmp_position.x() + m_radius > 1.0f || tmp_position.x() - m_radius < -1.0f) {
		m_velocity = {-m_velocity.x(), m_velocity.y()};
	}
	if (tmp_position.y() + m_radius > 1.0f || tmp_position.y() - m_radius < -1.0f) {
		m_velocity = {m_velocity.x(), -m_velocity.y()};
	}
	m_position += m_velocity * dt;
}

void RectangleSprite::step(const float dt) {
	auto tmp_position = m_position + m_velocity * dt;
	if (tmp_position.x() + m_width / 2 > 1.0f || tmp_position.x() - m_width / 2 < -1.0f) {
		m_velocity = {-m_velocity.x(), m_velocity.y()};
	}
	if (tmp_position.y() + m_height / 2 > 1.0f || tmp_position.y() - m_height / 2 < -1.0f) {
		m_velocity = {m_velocity.x(), -m_velocity.y()};
	}
	m_position += m_velocity * dt;
}