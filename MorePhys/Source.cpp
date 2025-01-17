#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>

/** We need this to easily convert between pixel and real-world coordinates*/
static const float SCALE = 30.f;

/** Create the base for the boxes to land */
void CreateGround(b2World& World, float X, float Y);

/** Create the boxes */
void CreateBox(b2World& World, int MouseX, int MouseY);

/** Create the circles */
void CreateCircle(b2World& World, int MouseX, int MouseY);

int main()
{
	/** Prepare the window */
	sf::RenderWindow Window(sf::VideoMode(800, 600, 32), "Test");
	Window.setFramerateLimit(60);
	Window.setKeyRepeatEnabled(false);

	/** Prepare the world */
	b2Vec2 Gravity(0.f, 9.81f);
	b2World World(Gravity);
	CreateGround(World, 400.f, 500.f);

	/** Prepare textures */
	sf::Texture GroundTexture;
	sf::Texture BoxTexture;
	GroundTexture.loadFromFile("ground.png");
	BoxTexture.loadFromFile("circle.png");
	bool pendingRelease{ false };
	while (Window.isOpen())
	{

		sf::Event event;
		while (Window.pollEvent(event))
		{
			if (event.type == event.Closed)
				Window.close();
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) )
		{
			if ((pendingRelease == false))
			{
				int MouseX = sf::Mouse::getPosition(Window).x;
				int MouseY = sf::Mouse::getPosition(Window).y;
				CreateCircle(World, MouseX, MouseY);
				pendingRelease = true;
			}
			
		}
		else
		{
			pendingRelease = false;
		}
		World.Step(1 / 60.f, 8, 3);

		Window.clear(sf::Color::White);
		int BodyCount = 0;
		for (b2Body* BodyIterator = World.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
		{
			if (BodyIterator->GetType() == b2_dynamicBody)
			{
				sf::Sprite Sprite;
				Sprite.setTexture(BoxTexture);
				Sprite.setOrigin(16.f, 16.f);
				Sprite.setPosition(SCALE * BodyIterator->GetPosition().x, SCALE * BodyIterator->GetPosition().y);
				Sprite.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);
				Window.draw(Sprite);
				++BodyCount;
			}
			else
			{
				sf::Sprite GroundSprite;
				GroundSprite.setTexture(GroundTexture);
				GroundSprite.setOrigin(400.f, 8.f);
				GroundSprite.setPosition(BodyIterator->GetPosition().x * SCALE, BodyIterator->GetPosition().y * SCALE);
				GroundSprite.setRotation(180 / b2_pi * BodyIterator->GetAngle());
				Window.draw(GroundSprite);
			}
		}
		Window.display();
	}

	return 0;
}

void CreateBox(b2World& World, int MouseX, int MouseY)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(MouseX / SCALE, MouseY / SCALE);
	BodyDef.type = b2_dynamicBody;
	b2Body* Body = World.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox((32.f / 2) / SCALE, (32.f / 2) / SCALE);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.1f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);
}

void CreateCircle(b2World& World, int MouseX, int MouseY)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(MouseX / SCALE, MouseY / SCALE);
	BodyDef.type = b2_dynamicBody;
	b2Body* Body = World.CreateBody(&BodyDef);
	
	b2CircleShape Circle;
	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.0f;
	FixtureDef.friction = 0.7f;
	Circle.m_radius = 16.0f / SCALE;
	FixtureDef.shape = &Circle;
	Body->CreateFixture(&FixtureDef);
}

void CreateGround(b2World& World, float X, float Y)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(X / SCALE, Y / SCALE);
	BodyDef.type = b2_staticBody;
	//BodyDef.angle = -((11.0f * b2_pi) / 6.0f);
	b2Body* Body = World.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox((800.f / 2) / SCALE, (16.f / 2) / SCALE);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 0.f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);
}