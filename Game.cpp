#include "stdafx.h"
#include "Game.h"
#include <sstream>
#include <windows.h>

Game::Game()
{
	Reset();
}

void Game::Reset()
{
	Console::SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Console::CursorVisible(false);
	paddle.width = 12;
	paddle.height = 2;
	paddle.x_position = 32;
	paddle.y_position = 30;

	ball.visage = 'O';
	ball.color = ConsoleColor::Cyan;
	ResetBall();

	// TODO #2 - Add this brick and 4 more bricks to the vector

	brick.width = 10;
	brick.height = 2;
	brick.x_position = 5;
	brick.y_position = 5;
	brick.doubleThick = true;
	brick.color = ConsoleColor::DarkGreen;
	bricks.push_back(brick);

	brick.width = 10;
	brick.height = 2;
	brick.x_position = 20;
	brick.y_position = 5;
	brick.doubleThick = true;
	brick.color = ConsoleColor::DarkGreen;
	bricks.push_back(brick);

	brick.width = 10;
	brick.height = 2;
	brick.x_position = 35;
	brick.y_position = 5;
	brick.doubleThick = true;
	brick.color = ConsoleColor::DarkGreen;
	bricks.push_back(brick);

	brick.width = 10;
	brick.height = 2;
	brick.x_position = 50;
	brick.y_position = 5;
	brick.doubleThick = true;
	brick.color = ConsoleColor::DarkGreen;
	bricks.push_back(brick);

	brick.width = 10;
	brick.height = 2;
	brick.x_position = 65;
	brick.y_position = 5;
	brick.doubleThick = true;
	brick.color = ConsoleColor::DarkGreen;
	bricks.push_back(brick);
	
}

void Game::ResetBall()
{
	ball.x_position = paddle.x_position + paddle.width / 2;
	ball.y_position = paddle.y_position - 1;
	ball.x_velocity = rand() % 2 ? 1 : -1;
	ball.y_velocity = -1;
	ball.moving = false;
}

bool Game::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
		return false;

	if (GetAsyncKeyState(VK_RIGHT) && paddle.x_position < WINDOW_WIDTH - paddle.width)
		paddle.x_position += 2;

	if (GetAsyncKeyState(VK_LEFT) && paddle.x_position > 0)
		paddle.x_position -= 2;

	if (GetAsyncKeyState(VK_SPACE) & 0x1)
		ball.moving = !ball.moving;

	if (GetAsyncKeyState('R') & 0x1)
		Reset();

	ball.Update();
	CheckCollision();
	return true;
}

//  All rendering, including text, should occur in the Render function
void Game::Render() const
{
	Console::Lock(true);
	Console::Clear();
	
	paddle.Draw();
	ball.Draw();

	// TODO #3 - Update render to render all bricks
	for (int i = 0; i < bricks.size(); i++)
	{
		bricks.at(i).Draw();
	}

	if (bricks.empty()) {
		std::string text = "You win! Press R to play again.";

		HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		COORD coord;

		GetConsoleScreenBufferInfo(stdoutHandle, &csbi);
		int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

		coord.X = (short)((width - text.length()) / 2);
		coord.Y = (short)(height / 2);
		SetConsoleCursorPosition(stdoutHandle, coord);
		std::cout << text << std::endl;
	}

	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	for (int i = 0; i < bricks.size(); i++) {
		if (bricks.at(i).Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
		{
			bricks.at(i).color = ConsoleColor(bricks.at(i).color - 1);
			ball.y_velocity *= -1;

			// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
			for (auto it = bricks.begin(); it != bricks.end();)
			{
				if ((it->color) == Black)
				{
					it = bricks.erase(it);
				}
				else
				{
					++it;
				}
			}
		}
	}



	// TODO #6 - If no bricks remain, pause ball and display (render) victory text with R to reset
	if (bricks.empty()) {
		ball.x_velocity *= 0;
		ball.y_velocity *= 0;
		Render();
	}


	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
	{
		ball.y_velocity *= -1;
	}

	// TODO #7 - If ball touches bottom of window, pause ball and display (render) defeat text with R to reset
}
