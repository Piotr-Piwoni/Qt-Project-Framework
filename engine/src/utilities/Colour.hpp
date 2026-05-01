#pragma once
#include <algorithm>
#include <cstdint>
#include <string_view>
#include <format>

namespace ChronicleEngine::Utils
{
class Channel
{
public:
	Channel() = default;
	explicit Channel(std::uint8_t value) : Value(value) {}
	explicit Channel(int value) : Value(Clamp(value)) {}
	explicit Channel(float value) : Value(Clamp01(value)) {}

	// ── Conversions ────────────────────────────────────────
	explicit operator std::uint8_t() const { return Value; }
	explicit operator int() const { return static_cast<int>(Value); }
	explicit operator float() const { return static_cast<float>(Value) / 255.0f; }

	[[nodiscard]] int AsInt() const { return static_cast<int>(Value); }
	[[nodiscard]] float AsFloat01() const { return static_cast<float>(Value) / 255.0f; }


	// ── Operators ───────────────────────────
	[[nodiscard]] Channel operator+(int v) const { return Channel(Clamp(AsInt() + v)); }
	[[nodiscard]] Channel operator-(int v) const { return Channel(Clamp(AsInt() - v)); }

	[[nodiscard]] Channel operator+(float v) const
	{
		return Channel(Clamp01(AsFloat01() + v));
	}

	[[nodiscard]] Channel operator-(float v) const
	{
		return Channel(Clamp01(AsFloat01() - v));
	}

	Channel& operator=(int v)
	{
		Value = Clamp(v);
		return *this;
	}

	Channel& operator=(float v)
	{
		Value = Clamp01(v);
		return *this;
	}

	bool operator==(const Channel& other) const
	{
		return Value == other.Value;
	}

	bool operator!=(const Channel& other) const
	{
		return Value != other.Value;
	}

	bool operator==(int v) const
	{
		return Value == Clamp(v);
	}

	bool operator!=(int v) const
	{
		return Value != Clamp(v);
	}

	bool operator==(float v) const
	{
		return Value == Clamp01(v);
	}

	bool operator!=(float v) const
	{
		return Value != Clamp01(v);
	}

private:
	static std::uint8_t Clamp(int value)
	{
		return static_cast<std::uint8_t>(std::clamp(value, 0, 255));
	}

	static std::uint8_t Clamp01(float value)
	{
		return static_cast<std::uint8_t>(std::clamp(value, 0.0f, 1.0f) * 255.0f);
	}

private:
	std::uint8_t Value{255};
};

class Colour
{
public:
	Colour() = default;
	Colour(int r, int g, int b, int a = 255) : R{r}, G{g}, B{b}, A{a} {}
	Colour(float r, float g, float b, float a = 1.f) : R{r}, G{g}, B{b}, A{a} {}
	explicit Colour(std::string_view hex) : Colour{FromHex(hex)} {}


	static Colour FromHex(std::string_view hex)
	{
		// Sanitise hex.
		if (!hex.empty() && hex[0] == '#') hex.remove_prefix(1);
		if (hex.size() != 6 && hex.size() != 8) return {};

		const auto parseByte = [](char high, char low) -> std::uint8_t {
			return static_cast<std::uint8_t>(HexToInt(high) * 16 + HexToInt(low));
		};

		std::uint8_t r = parseByte(hex[0], hex[1]);
		std::uint8_t g = parseByte(hex[2], hex[3]);
		std::uint8_t b = parseByte(hex[4], hex[5]);

		std::uint8_t a = 255;
		if (hex.size() == 8) a = parseByte(hex[6], hex[7]);

		return Colour{r, g, b, a};
	}

private:
	static std::uint8_t HexToInt(char hex)
	{
		if (hex >= '0' && hex <= '9') return hex - '0';
		if (hex >= 'a' && hex <= 'f') return hex - 'a' + 10;
		if (hex >= 'A' && hex <= 'F') return hex - 'A' + 10;
		return 0;
	}

public:
	Channel R{}, G{}, B{}, A{};
};
}


template<>
struct std::formatter<ChronicleEngine::Utils::Channel, char>
{
	static constexpr auto parse(std::format_parse_context& ctx)
	{
		return ctx.begin();
	}

	static auto format(const ChronicleEngine::Utils::Channel& c, std::format_context& ctx)
	{
		return std::format_to(ctx.out(), "{}", c.AsInt());
	}
};
