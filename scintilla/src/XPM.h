// Scintilla source code edit control
/** @file XPM.h
 ** Define a classes to hold image data in the X Pixmap (XPM) and RGBA formats.
 **/
// Copyright 1998-2003 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.
#pragma once

namespace Scintilla::Internal {

/**
 * Hold a pixmap in XPM format.
 */
class XPM final {
	int height = 1;
	int width = 1;
	int nColours = 1;
	std::vector<unsigned char> pixels;
	ColourRGBA colourCodeTable[256];
	char codeTransparent = ' ';
	ColourRGBA ColourFromCode(int ch) const noexcept;
	void FillRun(Surface *surface, int code, int startX, int y, int x) const;
public:
	explicit XPM(const char *textForm);
	explicit XPM(const char *const *linesForm);
	XPM(const XPM &) = default;
	XPM(XPM &&) = default;
	XPM &operator=(const XPM &) = default;
	XPM &operator=(XPM &&) = default;
	~XPM();
	void Init(const char *textForm);
	void Init(const char *const *linesForm);
	/// Decompose image into runs and use FillRectangle for each run
	void SCICALL Draw(Surface *surface, PRectangle rc);
	constexpr int GetHeight() const noexcept {
		return height;
	}
	constexpr int GetWidth() const noexcept {
		return width;
	}
	ColourRGBA PixelAt(int x, int y) const noexcept;
private:
	static std::vector<const char *>LinesFormFromTextForm(const char *textForm);
};

/**
 * A translucent image stored as a sequence of RGBA bytes.
 */
class RGBAImage final {
	int height;
	int width;
	float scale;
	std::vector<unsigned char> pixelBytes;
public:
	static constexpr size_t bytesPerPixel = 4;
	RGBAImage(int width_, int height_, float scale_, const unsigned char *pixels_);
	explicit RGBAImage(const XPM &xpm);
	RGBAImage(const RGBAImage &) = default;
	RGBAImage(RGBAImage &&) = default;
	RGBAImage &operator=(const RGBAImage &) = default;
	RGBAImage &operator=(RGBAImage &&) = default;
	~RGBAImage();
	constexpr int GetHeight() const noexcept {
		return height;
	}
	constexpr int GetWidth() const noexcept {
		return width;
	}
	constexpr float GetScale() const noexcept {
		return scale;
	}
	float GetScaledHeight() const noexcept {
		return height / scale;
	}
	float GetScaledWidth() const noexcept {
		return width / scale;
	}
	int CountBytes() const noexcept;
	const unsigned char *Pixels() const noexcept;
	void SetPixel(int x, int y, ColourRGBA colour) noexcept;
	static void BGRAFromRGBA(unsigned char *pixelsBGRA, const unsigned char *pixelsRGBA, size_t count) noexcept;
};

/**
 * A collection of RGBAImage pixmaps indexed by integer id.
 */
class RGBAImageSet final {
	typedef std::map<int, std::unique_ptr<RGBAImage>> ImageMap;
	ImageMap images;
	mutable int height;	///< Memorize largest height of the set.
	mutable int width;	///< Memorize largest width of the set.
public:
	RGBAImageSet() noexcept;
	// Deleted so RGBAImageSet objects can not be copied.
	RGBAImageSet(const RGBAImageSet &) = delete;
	RGBAImageSet(RGBAImageSet &&) = delete;
	RGBAImageSet &operator=(const RGBAImageSet &) = delete;
	RGBAImageSet &operator=(RGBAImageSet &&) = delete;
	~RGBAImageSet();
	/// Remove all images.
	void Clear() noexcept;
	/// Add an image.
	void AddImage(int ident, std::unique_ptr<RGBAImage> image);
	/// Get image by id.
	RGBAImage *Get(int ident) const;
	/// Give the largest height of the set.
	int GetHeight() const noexcept;
	/// Give the largest width of the set.
	int GetWidth() const noexcept;
};

}
