#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofSetColor(255);

	this->number_of_targets = 500;
	for (int i = 0; i < this->number_of_targets; i++) {

		this->target_list.push_back(glm::vec2());
		this->color_list.push_back(glm::vec3(1, 1, 1));
	}

	this->shader.load("shader/shader.vert", "shader/shader.frag");
}

//--------------------------------------------------------------
void ofApp::update() {

	if (this->location_list.size() < this->number_of_targets) {

		for (int i = 0; i < 2; i++) {

			this->location_list.push_back(glm::vec2(ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight())));

			glm::vec3 tmp_color;
			switch ((int)ofRandom(3))
			{
			case 0:
				tmp_color += glm::vec3(0.66, 0.25, 0.25);
				break;
			case 1:
				tmp_color += glm::vec3(0.25, 0.65, 0.25);
				break;
			case 2:
				tmp_color += glm::vec3(0.25, 0.25, 0.65);				
				break;
			}
			this->location_color_list.push_back(tmp_color);
			this->life_list.push_back(250);
		}
	}

	int radius = 5;
	for (int i = this->location_list.size() - 1; i >= 0; i--) {

		this->life_list[i] -= 1;
		if (this->life_list[i] < 0) {

			this->location_list.erase(this->location_list.begin() + i);
			this->location_color_list.erase(this->location_color_list.begin() + i);
			this->life_list.erase(this->life_list.begin() + i);

			continue;
		}

		auto deg = ofMap(ofNoise(glm::vec3(this->location_list[i] * 0.01, ofGetFrameNum() * 0.003)), 0, 1, -360, 360);
		this->location_list[i] = this->location_list[i] + glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD));
	}

	for (int i = 0; i < this->number_of_targets; i++) {

		if (i < this->location_list.size()) {

			this->target_list[i] = this->location_list[i];
			this->color_list[i] = this->location_color_list[i];
		}
		else {

			this->target_list[i] = glm::vec2();
			this->color_list[i] = glm::vec3(0);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofFill();
	this->shader.begin();
	this->shader.setUniform1f("time", ofGetElapsedTimef());
	this->shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	this->shader.setUniform2fv("targets", &this->target_list[0].x, this->number_of_targets);
	this->shader.setUniform3fv("colors", &this->color_list[0].x, this->number_of_targets);

	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	this->shader.end();
}

//--------------------------------------------------------------
int main() {

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(720, 720);
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}