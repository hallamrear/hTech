#pragma once

class Vector3
{
public:
	float X;
	float Y;
	float Z;

	Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	~Vector3();

	float Dot(const Vector3& other);
	void Normalise();
	Vector3 GetNormalized() const;
	Vector3 Cross(const Vector3& other);

	float GetMagnitude() const;
	float GetMagnitudeSquared() const;

	static float Dot(const Vector3& A, const Vector3& B);
	static Vector3 Cross(const Vector3& A, const Vector3& B);

	//this + 3f
	Vector3 operator+(const Vector3& other)
	{
		Vector3 ans;
		ans.X = X + other.X;
		ans.Y = Y + other.Y;
		ans.Z = Z + other.Z;
		return ans;
	};

	//this += 3f
	Vector3& operator+=(Vector3& other)
	{
		X += other.X;
		Y += other.Y;
		Z += other.Z;
		return *this;
	};

	//this - 3f
	Vector3 operator-(const Vector3& other)
	{
		Vector3 ans;
		ans.X = X - other.X;
		ans.Y = Y - other.Y;
		ans.Z = Z - other.Z;
		return ans;
	};

	//this -= 3f
	Vector3& operator-=(Vector3& other)
	{
		X -= other.X;
		Y -= other.Y;
		Z -= other.Z;
		return *this;
	};

	//this * 3f
	Vector3 operator*(const Vector3& other)
	{
		Vector3 ans;
		ans.X = X * other.X;
		ans.Y = Y * other.Y;
		ans.Z = Z * other.Z;
		return ans;
	};

	//this * (float)
	Vector3 operator*(const float& other)
	{
		Vector3 ans;
		ans.X = X * other;
		ans.Y = Y * other;
		ans.Z = Z * other;
		return ans;
	};

	//this * (int)
	Vector3 operator*(const int& other)
	{
		Vector3 ans;
		ans.X = X * other;
		ans.Y = Y * other;
		ans.Z = Z * other;
		return ans;
	};

	//this * (double)
	Vector3 operator*(const double& other)
	{
		Vector3 ans;
		ans.X = X * (float)other;
		ans.Y = Y * (float)other;
		ans.Z = Z * (float)other;
		return ans;
	};

	//(float) * this
	friend Vector3 operator*(float& scalar, const Vector3& vector) {
		Vector3 ans;
		ans.X = vector.X * scalar;
		ans.Y = vector.Y * scalar;
		ans.Z = vector.Z * scalar;
		return ans;
	};

	//(int) * this
	friend Vector3 operator*(int& scalar, const Vector3& vector) {
		Vector3 ans;
		ans.X = vector.X * scalar;
		ans.Y = vector.Y * scalar;
		ans.Z = vector.Z * scalar;
		return ans;
	};

	//(double) * this
	friend Vector3 operator*(double& scalar, const Vector3& vector) {
		Vector3 ans;
		ans.X = vector.X * (float)scalar;
		ans.Y = vector.Y * (float)scalar;
		ans.Z = vector.Z * (float)scalar;
		return ans;
	};

	//this *= 3f
	Vector3 operator*=(const Vector3& other)
	{
		X *= other.X;
		Y *= other.Y;
		Z *= other.Z;
		return *this;
	};

	//this *= (float)
	Vector3 operator*=(const float& other)
	{
		X *= other;
		Y *= other;
		Z *= other;
		return *this;
	};

	//this *= (int)
	Vector3 operator*=(const int& other)
	{
		X *= other;
		Y *= other;
		Z *= other;
		return *this;
	};

	//this *= (double)
	Vector3 operator*=(const double& other)
	{
		X *= (float)other;
		Y *= (float)other;
		Z *= (float)other;
		return *this;
	};

	//3f / 3f
	Vector3 operator/(const Vector3& other)
	{
		Vector3 ans;
		ans.X = X / other.X;
		ans.Y = Y / other.Y;
		ans.Z = Z / other.Z;
		return ans;
	};

	//3f / (float)
	Vector3 operator/(const float& other)
	{
		Vector3 ans;
		ans.X = X / other;
		ans.Y = Y / other;
		ans.Z = Z / other;
		return ans;
	};

	//3f / (int)
	Vector3 operator/(const int& other)
	{
		Vector3 ans;
		ans.X = X / other;
		ans.Y = Y / other;
		ans.Z = Z / other;
		return ans;
	};

	//3f / (double)
	Vector3 operator/(const double& other)
	{
		Vector3 ans;
		ans.X = X / (float)other;
		ans.Y = Y / (float)other;
		ans.Z = Z / (float)other;
		return ans;
	};

	//this /= 3f
	Vector3 operator/=(const Vector3& other)
	{
		X /= other.X;
		Y /= other.Y;
		Z /= other.Z;
		return *this;
	};


	//3f /= (float)
	Vector3 operator/=(const float& other)
	{
		X /= other;
		Y /= other;
		Z /= other;
		return *this;
	};

	//3f /= (int)
	Vector3 operator/=(const int& other)
	{
		X /= other;
		Y /= other;
		Z /= other;
		return *this;
	};

	//3f /= (double)
	Vector3 operator/=(const double& other)
	{
		X /= (float)other;
		Y /= (float)other;
		Z /= (float)other;
		return *this;
	};
};