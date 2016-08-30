#include "Application2D.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <thread>

#include "SpriteBatch.h"
#include "Texture.h"
#include "Font.h"
#include "fmod_errors.h"


Application2D::Application2D() {

}

Application2D::~Application2D() {

}

bool Application2D::InitAudio()
{
	FMOD_RESULT result;
	fmodSystem = NULL;

	result = FMOD::System_Create(&fmodSystem);

	srand(time(NULL));

	SongFiles.push_back("./bin/sounds/BKG/Maria.ogg");
	SongFiles.push_back("./bin/sounds/BKG/Souls.ogg");
	SongFiles.push_back("./bin/sounds/BKG/Cinder.ogg");


	if (result != FMOD_OK)
	{
		printf("FMOD Error! (%d) %s\n", result, FMOD_ErrorString(result));
		return false;
	}

	result = fmodSystem->init(512, FMOD_INIT_NORMAL, 0);

	if (result != FMOD_OK)
	{
		printf("FMOD Error! (%d) %s\n", result, FMOD_ErrorString(result));
		return false;
	}

	result = fmodSystem->createStream(SongFiles.at(rand() % SongFiles.size()), FMOD_DEFAULT, 0, &pSound);
	if (result != FMOD_OK)
	{
		printf("FMOD Error! (%d) %s\n", result, FMOD_ErrorString(result));
		return false;
	}
	result = fmodSystem->createChannelGroup("LaChannelGroupe", &pChannelGroup);
	pChannelGroup->setVolume(.5f);
	pChannelGroup->setMode(FMOD_LOOP_NORMAL);
	if (result != FMOD_OK)
	{
		printf("FMOD Error! (%d) %s\n", result, FMOD_ErrorString(result));
		return false;
	}
	result = fmodSystem->playSound(pSound, pChannelGroup, false, &pChannel);
	if (result != FMOD_OK)
	{
		printf("FMOD Error! (%d) %s\n", result, FMOD_ErrorString(result));
		return false;
	}
	return true;
}

bool Application2D::startup() {
	
	//Debug = true;

	useAudio = InitAudio();
	
	createWindow("A.I. Project", 1280, 720);
	
	m_spriteBatch = new SpriteBatch();
	std::vector<Texture*> tempVec;
	tempVec.push_back(new Texture("./bin/textures/Anims/Idle0.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Idle1.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Idle2.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Walk0.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Walk1.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Walk2.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Walk3.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Attack0.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Attack1.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Attack2.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Attack3.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Parry0.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Parry1.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Parry2.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Attack1 - Copy.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Attack3 - Copy.png"));

	CollisionBoxes = std::vector<CollisionBox*>();

	Agent1 = new Agent(glm::vec2(500, 350), 100, 100, true, tempVec, CollisionBoxes, fmodSystem);

	tempVec.clear();

	tempVec.push_back(new Texture("./bin/textures/Anims/Idle0 - Copy.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Idle1 - Copy.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Idle2 - Copy.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Walk0 - Copy.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Walk1 - Copy.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Walk2 - Copy.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Walk3 - Copy.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Attack0 - Copy.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Attack1 - Copy.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Attack2 - Copy.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Attack3 - Copy.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Parry0 - Copy.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Parry1 - Copy.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Parry2 - Copy.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Attack1.png"));
	tempVec.push_back(new Texture("./bin/textures/Anims/Attack3.png"));

	Agent2 = new Agent(glm::vec2(700, 350), 100, 100, false, tempVec, CollisionBoxes, fmodSystem);
	//Agent1->m_oAgentState = AgentState::ATTACK2;
	//Agent2->m_oAgentState = AgentState::FORWARD;
	m_texture = new Texture("./bin/textures/crate.png");

	m_font = new Font("./bin/font/consolas.ttf", 32);


	server = new Server(Agent1, Agent2);
	server->run();
	std::thread* serverThread = new std::thread(&Server::handleNetworkMessages, server);
	return true;
}

void Application2D::shutdown() {

	delete m_font;
	delete m_texture;
	delete m_spriteBatch;

	destroyWindow();
}

bool Application2D::update(float deltaTime) {
	Framerate = 1.0 / deltaTime;
	//printf("Framerate: %i\n", Framerate);
	//printf("Update Frequency: %d\n", (Framerate / 30));
	
	if (Framerate > 120 || Framerate <= 30)
	{
		Framerate = 30;
	}
	float timeScale = Framerate / 30;

	if (Agent1->m_iHealth <= 0 || Agent2->m_iHealth <= 0)
	{
		if (GameOver == false)
		{
			if (Agent2->m_iHealth <= 0)
			{
				Agent1->Score += 1;
			}
			else
			{
				Agent2->Score += 1;
			}
		}
		
		GameOver = true;
		
	}
	if (GameOver)
	{
		RestartTimer += deltaTime;
		timeScale = 2;
	}
	if (RestartTimer >= RestartTime)
	{
		GameOver = false;
		RestartTimer = 0;
		Agent1->m_iHealth = 100;
		Agent1->m_iCurrStamina = Agent1->m_iMaxStamina;
		Agent1->m_oPos = glm::vec2(500, 350);
		Agent1->m_oAgentState = IDLE;
		Agent1->m_iStateIndex = 0;
		Agent1->BodyBox->Active = true;

		Agent2->m_iHealth = 100;
		Agent2->m_iCurrStamina = Agent2->m_iMaxStamina;
		Agent2->m_oPos = glm::vec2(700, 350);
		Agent2->m_oAgentState = IDLE;
		Agent2->m_iStateIndex = 0;
		Agent2->BodyBox->Active = true;

	}
	// close the application if the window closes or we press escape
	if (hasWindowClosed() || isKeyPressed(GLFW_KEY_ESCAPE))
		return false;

	if (isKeyPressed(GLFW_KEY_1))
	{
		useAudio = false;
	}
	if (isKeyPressed(GLFW_KEY_2))
	{
		useAudio = true;
	}


	if (Agent1->m_uiClientID == -1)
	{
		if (isMouseButtonPressed(0))
		{
			Agent1->ChangeState(AgentState::ATTACK1);
		}
		else if (isMouseButtonPressed(1))
		{
			Agent1->ChangeState(AgentState::ATTACK2);
		}
		else if (isKeyPressed(GLFW_KEY_A))
		{
			Agent1->ChangeState(AgentState::BACK);
		}
		else if (isKeyPressed(GLFW_KEY_D))
		{
			Agent1->ChangeState(AgentState::FORWARD);
		}
		else
		{
			Agent1->ChangeState(AgentState::IDLE);
		}
	}
	if (Agent2->m_uiClientID == -1)
	{
		if (isKeyPressed(GLFW_KEY_COMMA))
		{
			Agent2->ChangeState(AgentState::ATTACK1);
		}
		else if (isKeyPressed(GLFW_KEY_PERIOD))
		{
			Agent2->ChangeState(AgentState::ATTACK2);
		}
		else if (isKeyPressed(GLFW_KEY_LEFT))
		{
			Agent2->ChangeState(AgentState::FORWARD);
		}
		else if (isKeyPressed(GLFW_KEY_RIGHT))
		{
			Agent2->ChangeState(AgentState::BACK);
		}
		else
		{
			Agent2->ChangeState(AgentState::IDLE);
		}
	}

	Agent1->Update(Framerate / 30);
	Agent2->Update(Framerate / 30);

	if (Agent1->m_oPos.x < Agent1->BodyBox->m_oSize.x / 2)
	{
		Agent1->m_oPos.x = Agent1->BodyBox->m_oSize.x / 2;
	}
	else if (Agent1->m_oPos.x > 1280 - Agent1->BodyBox->m_oSize.x / 2)
	{
		Agent1->m_oPos.x = 1280 - Agent1->BodyBox->m_oSize.x / 2;
	}

	if (Agent2->m_oPos.x < Agent2->BodyBox->m_oSize.x / 2)
	{
		Agent2->m_oPos.x = Agent2->BodyBox->m_oSize.x / 2;
	}
	else if (Agent2->m_oPos.x > 1280 - Agent2->BodyBox->m_oSize.x / 2)
	{
		Agent2->m_oPos.x = 1280 - Agent2->BodyBox->m_oSize.x / 2;
	}

	

	for each(CollisionBox* cBox in CollisionBoxes)
	{
		if (cBox->Active)
		{
			cBox->Update();
		}
		for each(CollisionBox* otherBox in CollisionBoxes)
		{
			if(cBox->m_eType == BoxType::BODY && otherBox->m_eType == BoxType::BODY)
			{
				if (cBox->CheckCollision(otherBox))
				{
					float dist = cBox->m_oPos.x - otherBox->m_oPos.x;
					dist = abs(dist);
					double adjustPos = cBox->m_oSize.x - dist;
					otherBox->Owner->m_oPos.x += adjustPos / 2;
					otherBox->Update();
					cBox->Owner->m_oPos.x -= adjustPos / 2;
					cBox->Update();
				}
			}else if (otherBox->Active && cBox->Active)
			{
				if (otherBox->Owner != cBox->Owner)
				{
					if (cBox->CheckCollision(otherBox))
					{
						if (cBox->m_eType == BoxType::ATTACK && otherBox->m_eType == BoxType::BODY)
						{
							otherBox->Owner->ChangeState(HURT);
						}
						else if (cBox->m_eType == BoxType::ATTACK && otherBox->m_eType == BoxType::PARRY)
						{
							cBox->Owner->ChangeState(STUNNED);
							
						}
					}
				}
			}
		}
	}

	if (useAudio)
	{
		fmodSystem->update();
	}
	server->BroadcastGameData();
	// the applciation closes if we return false
	return true;
}

void Application2D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_spriteBatch->begin();

	m_spriteBatch->setSpriteColor(1, 1, 1, 1);
	Agent1->Draw(m_spriteBatch);
	Agent2->Draw(m_spriteBatch);
	m_spriteBatch->setSpriteColor(1, 1, 1, 1);
	//m_spriteBatch->drawSprite(m_texture, 200, 200, 100, 100);
	m_spriteBatch->drawSprite(nullptr, 1280, 720, 1280*2, 720*2, 0, 100);
	float hpPercent = Agent1->m_iHealth * 0.01f;
	m_spriteBatch->setSpriteColor(1, 0.3f, 0.3f, 1);
	m_spriteBatch->drawLine(20, 700, 500 * hpPercent, 700, 10, 0);
	m_spriteBatch->setSpriteColor(0, 0, 0, 1);
	m_spriteBatch->drawLine(20, 700, 500, 700, 10, 1);
	float a = (Agent2->m_iHealth * 0.01f);
	hpPercent = 1 - a;
	m_spriteBatch->setSpriteColor(1, 0.3f, 0.3f, 1);
	m_spriteBatch->drawLine(780 + (480 * hpPercent), 700, 1260, 700, 10, 0);
	m_spriteBatch->setSpriteColor(0, 0, 0, 1);
	m_spriteBatch->drawLine(780, 700, 1260, 700, 10, 1);

	float stamPercent = (Agent1->m_iCurrStamina * 0.01f);
	m_spriteBatch->setSpriteColor(0.3, 1.f, 0.3f, 1);
	int MaxPos = 450 * stamPercent;
	if (MaxPos < 20)
	{
		MaxPos = 20;
	}
	m_spriteBatch->drawLine(20, 680, MaxPos, 680, 10, 0);
	m_spriteBatch->setSpriteColor(0, 0, 0, 1);
	m_spriteBatch->drawLine(20, 680, 450, 680, 10, 1);
	a = (Agent2->m_iCurrStamina * 0.01f);
	stamPercent = 1 - a;

	MaxPos = 830 + (430 * stamPercent);
	if (MaxPos > 1260)
	{
		MaxPos = 1260;
	}
	m_spriteBatch->setSpriteColor(0.3, 1.f, 0.3f, 1);
	m_spriteBatch->drawLine(830 + (430 * stamPercent), 680, 1260, 680, 10, 0);
	m_spriteBatch->setSpriteColor(0, 0, 0, 1);
	m_spriteBatch->drawLine(830,680, 1260, 680, 10, 1);

	char score[3];
	itoa(Agent1->Score, score, 10);
	m_spriteBatch->drawText(m_font, score, 540, 680, 0);
	itoa(Agent2->Score, score, 10);
	m_spriteBatch->drawText(m_font, score, 720, 680, 0);

	if (GameOver)
	{
		if (Agent1->m_iHealth <= 0)
		{
			m_spriteBatch->drawText(m_font, "Player 2 Wins", 200, 300);
		}
		else if (Agent2->m_iHealth <= 0)
		{
			m_spriteBatch->drawText(m_font, "Player 1 Wins", 200, 300);
		}
	}



	//m_spriteBatch->setSpriteColor(0, 1, 1, 1);
	//m_spriteBatch->drawText(m_font, "OMG BBQ!", 200, 400);
	//m_spriteBatch->drawText(m_font, "Yeaaahhhhh", 200, 300);
	if (Debug)
	{
		for each(CollisionBox* cBox in CollisionBoxes)
		{
			if (cBox->Active)
			{
				cBox->Draw(m_spriteBatch);
			}
		}
	}

	// done drawing sprites
	m_spriteBatch->end();	
}