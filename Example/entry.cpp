#include <hTech/Game.h>
#include <hTech\StateDirector.h>
#include <hTech\GameStates.h>
#include <hTech\TextElement.h>
#include <hTech\Vector2.h>
#include <hTech\Transform.h>
#include <hTech\InputManager.h>
#include <hTech\BoundingSphere.h>
#include <hTech/BoundingBox.h>
#include <hTech/OrientedBoundingBox.h>
#include <hTech\BoundingPolygon.h>
#include <hTech\Settings.h>
#include <hTech\Rigidbody.h>
#include <hTech\Camera.h>

#include "hTech\SDL\include\SDL.h"

class testColl : public Rigidbody
{
private:
	int id;
	TextElement* text;
	CollisionManifold manifold;

public:
	float speed = 1.0f;

	testColl(int i, float s = 1.0f) : Rigidbody("", Transform(), PhysicsProperties(100.0f, 0.5f, 1000.0f, 0.47f, true, false, false))
	{
		speed = s;
		id = i;

		switch (i)
		{
			case 0:
				mCollider = new BoundingSphere(mTransform.Position, 64.0f);
				break;
			case 1:
				mTransform.AdjustPosition(Vector2f(1.0f, 0.0f));
				mCollider = new BoundingBox(mTransform.Position, 128.0f, 128.0f);
				break;
			case 2:
				mTransform.AdjustPosition(Vector2f(0.0f, -200.0f));
				mCollider = new OrientedBoundingBox(mTransform.Position, mTransform.Rotation, 800.0f, 300.0f);
				break;

			case 3:
				mTransform.Rotation = rand() % 360;
				mTransform.AdjustPosition(Vector2f(-200.0f, 1.0f));
				mCollider = new OrientedBoundingBox(mTransform.Position, mTransform.Rotation, 32.0f, 32.0f);
				break;

			case 4:
			{
				std::vector<Vector2f> points;
				Vector2f p = Vector2f(0.0f, 100.0f);
				points.push_back(p);

				for (int i = 0; i < 4; i++)
				{
					p = HelperFunctions::RotatePointAroundOriginDegrees(p, 72.0f, Vector2f(0.0f, 0.0f));
					points.push_back(p);
				}
				mCollider = new BoundingPolygon(mTransform.Position, mTransform.Rotation, points, points.size());
			}
			break;
			case 5:
			{
				std::vector<Vector2f> points;
				Vector2f p = Vector2f(0.0f, 100.0f);
				points.push_back(p);

				for (int i = 0; i < 11; i++)
				{
					p = HelperFunctions::RotatePointAroundOriginDegrees(p, 30.0f, Vector2f(0.0f, 0.0f));
					points.push_back(p);
				}
				mCollider = new BoundingPolygon(mTransform.Position, mTransform.Rotation, points, points.size());
			}
			break;
				
			case 6:
			{
				std::vector<Vector2f> points;
				Vector2f p = Vector2f(0.0f, 25.0f);
				points.push_back(p);

				for (int i = 0; i < 23; i++)
				{
					p = HelperFunctions::RotatePointAroundOriginDegrees(p, 15.0f, Vector2f(0.0f, 0.0f));
					points.push_back(p);
				}
				mCollider = new BoundingPolygon(mTransform.Position, mTransform.Rotation, points, points.size());
			}
			break;

			case 7:
			{
				std::vector<Vector2f> points;
				Vector2f p = Vector2f(0.0f, 200.0f);
				points.push_back(p);

				for (int i = 0; i < 2; i++)
				{
					p = HelperFunctions::RotatePointAroundOriginDegrees(p, 120.0f, Vector2f(0.0f, 0.0f));
					points.push_back(p);
				}
				mCollider = new BoundingPolygon(mTransform.Position, mTransform.Rotation, points, points.size());

			}
			break;

			case 8:
			{
				mCollider = new OrientedBoundingBox(mTransform.Position, mTransform.Rotation, 2000.0f, 100.0f);
				mIsStatic = true;
			}
			break;


			case 9:
			{	
				std::vector<Vector2f> points;
				Vector2f p = Vector2f(0.0f, 0.0f);
				points.push_back(p);
				p = Vector2f(1000.0f, 0.0f);
				points.push_back(p);
				p = Vector2f(1000.0f, 200.0f);
				points.push_back(p);
				p = Vector2f(900.0f, 100.0f);
				points.push_back(p);
				p = Vector2f(800.0f, 200.0f);
				points.push_back(p);
				p = Vector2f(700.0f, 100.0f);
				points.push_back(p);
				p = Vector2f(600.0f, 200.0f);
				points.push_back(p);
				p = Vector2f(500.0f, 100.0f);
				points.push_back(p);
				p = Vector2f(400.0f, 200.0f);
				points.push_back(p);
				p = Vector2f(300.0f, 100.0f);
				points.push_back(p);
				p = Vector2f(200.0f, 200.0f);
				points.push_back(p);
				p = Vector2f(100.0f, 100.0f);
				points.push_back(p);
				p = Vector2f(0.0f, 200.0f);
				points.push_back(p);

				mCollider = new BoundingPolygon(mTransform.Position, mTransform.Rotation, points, points.size());
				speed = 0.0f;
			}
			break;
		}

		SetGravityEnabled(false);
		//mCollider->IsOverlap = true;

		text = new TextElement(Transform(), "");
	}

	void Update(double dt)
	{
		if (id > 1)
			mTransform.AdjustRotation(dt * speed);

		if (text)
		{
			if (manifold.HasCollided)
			{
				text->SetPosition(mTransform.Position + Vector2f(0.0f, 100.0f));
				std::string str = "";
				str += "Depth: " + std::to_string(manifold.Depth) + "\n";
				str += "Normal X:" + std::to_string(manifold.Normal.X) + ", Y:" + std::to_string(manifold.Normal.Y) + "\n";
				text->SetString(str);
				text->Update(dt);
			}
		}

		if (mCollider)
		{
			mCollider->Update(dt);
		}
	}

	void Render(SDL_Renderer& renderer)
	{
		if (text)
		{
			text->Render();
		}

		if(manifold.ObjA && manifold.ObjB)
		{
			Vector2f one = Camera::WorldToScreen(manifold.ObjA->GetTransform().Position);
			Vector2f two = Camera::WorldToScreen(manifold.ObjA->GetTransform().Position + (manifold.Normal.GetNormalized() * manifold.Depth));

			int x1, y1, x2, y2;
			x1 = one.X;
			y1 = one.Y;
			x2 = two.X;
			y2 = two.Y;

			SDL_SetRenderDrawColor(&renderer, 255, 0, 0, 255);
			SDL_RenderDrawLine(&renderer, x1, y1, x2, y2);

			one = Camera::WorldToScreen(manifold.ObjB->GetTransform().Position);
			two = Camera::WorldToScreen(manifold.ObjB->GetTransform().Position + ((manifold.Normal.GetNormalized() * -1) * manifold.Depth));
			x1 = one.X;
			y1 = one.Y;
			x2 = two.X;
			y2 = two.Y;
			SDL_SetRenderDrawColor(&renderer, 0, 255, 0, 255);
			SDL_RenderDrawLine(&renderer, x1, y1, x2, y2);
		}

		if (mCollider)
		{
			mCollider->Render(renderer);
		}
	}

	void Render()
	{

	}

	void OnOverlap(const CollisionManifold& manifold, Rigidbody& other)
	{
		this->manifold = manifold;
	}
};


class TestState : public GameState
{
	TextElement* text;
	testColl* a;
	testColl* b;
	testColl* c;
	bool Gravity = false;

	std::vector<testColl*> test;

	void Start()
	{
		text = new TextElement(Transform(Vector2f(0.0f, 64.0f)), "Hello, World!");
		test = std::vector<testColl*>();
		//a = new testColl(0);
		b = new testColl(7, 200.0f);
		c = new testColl(9, 50.0f);

		test.push_back(new testColl(8, 0.0f));
		test.back()->GetTransform().AdjustPosition(Vector2f(0.0f, -350.0f));

		for (int i = 0; i < 10; i++)
		{
			test.push_back(new testColl(6, 0.0f));
			test.back()->GetTransform().AdjustPosition(Vector2f(rand() % 50 - 25, -200.0f));
		}

		Settings::Get()->SetDrawColliders(true);

		InputManager::Bind(IM_KEY_CODE::IM_KEY_SPACE, IM_KEY_STATE::IM_KEY_PRESSED, [this]()
			{
				Gravity = !Gravity;

				//if (a) a->SetGravityEnabled(Gravity);
				//if (b) b->SetGravityEnabled(Gravity);
				//if (c) c->SetGravityEnabled(Gravity);

				if (Gravity)
				{
					if (b)
						b->speed = 200.0f;
				}
				else
				{
					if(b)
						b->speed = 0.0f;
				}
			});

		InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_LEFT_CLICK, IM_KEY_STATE::IM_KEY_PRESSED, [this]()
			{
				if (c)
				{
					c->GetTransform().Position = InputManager::Get()->GetMouseWorldPosition();
					c->AddVelocity(Vector2f(b->GetVelocity()) * -1.0f);
				}
			});

		InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_RIGHT_CLICK, IM_KEY_STATE::IM_KEY_HELD, [this]()
			{
				if (b)
				{
					b->GetTransform().Position = InputManager::Get()->GetMouseWorldPosition();
					b->AddVelocity(Vector2f(b->GetVelocity()) * -1.0f);
				}
			});


		InputManager::Bind(IM_KEY_CODE::IM_KEY_LEFT_ARROW, IM_KEY_STATE::IM_KEY_HELD, [this]()
			{
				//if (b) b->GetTransform().AdjustPosition(Vector2f(-5.0f, 0.0f));

				if (b) b->AddVelocity(Vector2f(-1.0f, 0.0f));

			});
		InputManager::Bind(IM_KEY_CODE::IM_KEY_RIGHT_ARROW, IM_KEY_STATE::IM_KEY_HELD, [this]()
			{
				//if (b) b->GetTransform().AdjustPosition(Vector2f(5.0f, 0.0f));
				if (b) b->AddVelocity(Vector2f(1.0f, 0.0f));
			});
		InputManager::Bind(IM_KEY_CODE::IM_KEY_UP_ARROW, IM_KEY_STATE::IM_KEY_HELD, [this]()
			{
				//if (b) b->GetTransform().AdjustPosition(Vector2f(0.0f, 5.0f));
				if (b) b->AddVelocity(Vector2f(0.0f, 1.0f));
			});
		InputManager::Bind(IM_KEY_CODE::IM_KEY_DOWN_ARROW, IM_KEY_STATE::IM_KEY_HELD, [this]()
			{
				//if (b) b->GetTransform().AdjustPosition(Vector2f(0.0f, -5.0f));
				if (b) b->AddVelocity(Vector2f(0.0f, -1.0f));
			});
	}

	void Update(double dt)
	{
		text->SetString(std::to_string(Gravity));
		text->SetString("X : " + std::to_string(b->GetVelocity().X) + " Y: " + std::to_string(b->GetVelocity().Y));
		
		for (auto itr : test)
			itr->Update(dt);

		if (a) a->Update(dt);
		if (b) b->Update(dt);
		if (c) c->Update(dt);
		text->Update(dt);
	}

	void Render(SDL_Renderer& renderer)
	{
		text->Render();
		if (a) 
			a->Render(renderer);

		if (c) 
			c->Render(renderer);
		if (b)
			b->Render(renderer);

		for (auto itr : test)
			itr->Render(renderer);
	}

	void End()
	{
		delete text; text = nullptr;
	}
};


int main(int argc, char* argv[])
{
	WindowDetails details;
	details.dimensions = Vector2f(1280.0f, 720.0f);
	details.title = "Hello, World!";
	details.position = Vector2f(200.0f, 200.0f);

	Game* game = new Game();
	game->Initialise(argc, argv, details);

	StateDirector::SetupState(GameStateIdentifier::GAME_STATE_1, new TestState());
	StateDirector::SetState(GameStateIdentifier::GAME_STATE_1);

	if (game->GetIsInitialised())
	{
		game->Start();
	}

	return 0;
}