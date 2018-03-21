#pragma once
#include <iostream>
#include <irrKlang.h>
#include <iostream>
#include <map>
#include <glm/glm.hpp>

using namespace irrklang;
using namespace std;
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

class AudioSystem {
private:

	//map<string, irrklang::ISoundSource*> soundSources;
	//std::map<string, irrklang::ISoundSource*>::iterator it;
	vector<ISoundSource*> sources;
public:
	ISoundEngine* engine;
	AudioSystem() {
		this->engine = createIrrKlangDevice();

	}
	~AudioSystem() {
		// delete the sound sources
		engine->removeAllSoundSources();
		//soundSources.clear();

		// delete the sound engine
		engine->drop();
	}

	// play single sound in 2D
	void PlaySound2D(string path) {
		if (!path.empty()) {
			const char* _tmpChar = path.c_str();
			engine->play2D(_tmpChar);
		}
	}

	// play looping sound in 2D
	void PlayLoopingSound2D(string path) {
		if (!path.empty()) {
			const char* _tmpChar = path.c_str();
			engine->play2D(_tmpChar, true);
		}
	}
	// play single sound source in 3D
	void PlaySound3D(ISoundSource* name, glm::vec3 position) {

		vec3df _pos(position.x, position.y, position.z);
		engine->play3D(name, _pos);

	}

	// play looping sound source in 3D
	void PlayLoopingSound3D(ISoundSource* name, glm::vec3 position) {

		vec3df _pos(position.x, position.y, position.z);
		engine->play3D(name, _pos, true);

	}

	// add a sound source (i.e. explosion/ walk)
	ISoundSource* AddSoundSource(string path, float _volume) {
		if (!path.empty()) {
			const char* _tmpChar = path.c_str();
			ISoundSource* _source = engine->addSoundSourceFromFile(_tmpChar);
			_source->setDefaultVolume(_volume);
			sources.push_back(_source);
			//soundSources[_name] = _source;
			return _source;
		}
	}

	// set the listener position
	void UpdateListnerPosition(glm::vec3 position, glm::vec3 lookDir) {
		vec3df _pos(position.x, position.y, position.z);
		vec3df _dir(lookDir.x, lookDir.y, lookDir.z);
		if (!this->engine) {
			cout << "oh no!" << endl;
		}
		else {
			this->engine->setListenerPosition(vec3df(0, 0, 0), vec3df(0, 0, 1));

		}
	}



};