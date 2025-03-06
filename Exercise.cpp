#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
/// Class describing a horizontal line of pixels within an image
class Line
{
public:

	/// The image X coordinate of the first pixel in the line
	/// (zero would indicate that the line begins in the leftmost column of the image).
	unsigned int m_X;

	/// The image Y coordinate of the line.
	/// (zero would indicate that the line lies in the topmost row of the image).
	unsigned int m_Y;

	/// The length of the line in pixels.
	unsigned int m_Length;

	/// Less than operator.
	/// Provided to clarify the ordering of Lines within an image.
	bool operator < (const Line& i_rOther) const
	{
		return (m_Y < i_rOther.m_Y) || ((m_Y == i_rOther.m_Y) && (m_X < i_rOther.m_X));
	}
};

/// TODO! Implement this class.
class BlobBuilder
{
public:

	/// Processes lines in an image in order to determine a blob index for each line.
	/// `i_rLineList` must be a sorted vector of Lines extracted from a single image.
	/// 'o_rBlobIndices` is the vector of Blob indices to be populated by this function.
	/// After calling this function, `o_rBlobIndices` should have the same number of
	/// elements as `i_rLineList` and be filled out such that `o_rBlobIndices[N]`
	/// provides the Blob index for the Line `i_rLineList[N]`.

	/*The below function assigns blob indices to each line in the i_rLineList
	The solution is inspired from the Agglomerative algorithms where we successively merge
	small clusters into big clusters based on some conditions.
	Here, we check the touching condition and based on that merge smaller group of lines into one blob.
	*/

	/*----------------------------- PERFORMANCE IMPROVEMENTS -------------------------------------
	1. Use more efficient data structures, such as hash maps or trees, to store blob indices and 
	perform lookups more efficiently.
	*/
	void Process(const std::vector< Line >& i_rLineList,
		std::vector< unsigned int >& o_rBlobIndices)
	{
		bool touched = false;
		int visitedBlobId;

		/* Initialize blob index mapping to hold lines in their respective blob buckets.
		Each blob bucket represents a group of touching lines.
		Example:
		blob bucket     lines
			0				0, 1, 6, 11
			1				2, 7, 15
		*/
		std::vector<std::vector<int>> blobIndexMapping;

		// Initialize blob indices with 0
		o_rBlobIndices.assign(i_rLineList.size(), 0);

		if (i_rLineList.empty())
			return;

		for (int i = 0; i < i_rLineList.size(); i++)
		{
			touched = false;
			for (int j = 0; j < i; j++)
			{
				// Only process consecutive rows
				if (std::abs(static_cast<int>(i_rLineList[i].m_Y) - static_cast<int>(i_rLineList[j].m_Y)) == 1)
				{
					if (IsTouching(i_rLineList[i], i_rLineList[j]))
					{
						// If lines touch, add them to the same blob bucket.
						if (!touched)
						{
							visitedBlobId = getBlobIDbyIndex(j, blobIndexMapping);
							blobIndexMapping[visitedBlobId].push_back(i);
							touched = true;
						}

						/*To handle bridge case : if bridge is found i.e. ith line touches multiple jth lines in the consecutive row then,
						1. add all the lines from jth line's blob bucket into ith line's blob bucket
						2. delete the jth line's blob bucket */

						else
						{
							int bridgeBlobId = getBlobIDbyIndex(j, blobIndexMapping);
							if (bridgeBlobId >= 0 && bridgeBlobId != visitedBlobId)
							{
								// Merge blob buckets if a bridge is found.
								if (bridgeBlobId < visitedBlobId)
								{
									int tId = visitedBlobId;
									visitedBlobId = bridgeBlobId;
									bridgeBlobId = tId;
								}
								std::copy(blobIndexMapping[bridgeBlobId].begin(), blobIndexMapping[bridgeBlobId].end(), std::back_inserter(blobIndexMapping[visitedBlobId]));
								blobIndexMapping.erase(std::next(blobIndexMapping.begin(), bridgeBlobId));
							}
						}
					}
				}
			}

			// If the line does not touch any other lines, create a new blob bucket.
			if (!touched)
			{
				std::vector<int> newBlob = { i };
				blobIndexMapping.push_back(newBlob);
			}
		}

		//To get the output in the Gold indices format
		for (int m = 0; m < blobIndexMapping.size(); m++)
		{
			for (auto n : blobIndexMapping[m])
			{
				o_rBlobIndices[n] = m;
			}
		}
	}

	// Retrieve the blob bucket ID of a given line.
	int getBlobIDbyIndex(int line, std::vector<std::vector<int>> blobIndexMapping)
	{
		for (int b = 0; b < blobIndexMapping.size(); b++)
		{
			auto it = std::find(blobIndexMapping[b].begin(), blobIndexMapping[b].end(), line);
			if (it != blobIndexMapping[b].end())
			{
				return b;
			}
		}
		return -1;
	}

	// Check if two lines are touching.
	bool IsTouching(const Line& line1, const Line& line2)
	{
		// Two lines touch if they share at least one pixel in the same column
		return (line1.m_X < (line2.m_X + line2.m_Length)) && ((line1.m_X + line1.m_Length) > line2.m_X);

	}
};


const std::vector< std::string > g_ImageSimple =
{
 "...............AAAA.............................................",
 ".....BBB......AAAAAA.....CCC......DDDDDDDDDDDDDDDDDDDDDDDD......",
 "...BBBBBBB.....AAAA....CCC.CCC....DDD...DDDDD...DDDD...DDD......",
 "..BBBBBBBBB..........CCC.....CCC..DDD...DDDDD...DDDD...DDD......",
 "...BBBBBBB.........CCC.CCC.....CCC..............................",
 ".....BBB.........CCC.....CCC.....CCC........EEE..FFFF..GGG......",
 "...................CCC.........CCC........EEE..FFF..FFF..GGG....",
 ".....................CCC.....CCC............EEE..FFF...GGG......",
 ".......................CCC.CCC..................................",
 "................................................................",
 ".....HHHHHHH.............IIIIIIIIII..............JJJJJJJJ.......",
 ".......HHH.........K.L...I........I......MM....JJJJJJJJJJJJ.....",
 ".....HHHHHHH.........L.....NNNNN..I......MM...JJJJJJJJJJJJJJ....",
 "..HHHHH.....................NNN...I............JJJJJJJJJJJJ.....",
 ".....HHHH.........................I..............JJJJJJJJ.......",
 "...HHH..HH................IIIIIIIII........................OOOOO",
};


const std::vector< std::string > g_ImageComplex =
{
 "........................AAAA............................................BBBBBBBBBBBBBBBBBBBBBBBBBB...",
 "...........CCC........AAAAAAA......DDDDD.....DDDD......................BB............................",
 ".......CCCCCCCCCCC.....AAAAA.....DDDDDDDDD.DDDDDDDDD..................BB..EEEEEE......FFFFFF.FFF.....",
 "......CCCCC...CCCCC.............DDDDDDDDDDDDDDDDDDDDD................BB......EEEEE..FFFFF..FFF.FF....",
 ".......CCCCCCCCCCC...............DDDDDDDDD.DDDDDDDDD......BBBB......BB........EEE.FFFFF......FFF.....",
 "...........CCC.....................DDDD......DDDD............BBB...BB........EEEEE...FFFF...FFFFFF...",
 "................GGG.HHHH.III.JJJJ..............................BB.BB......EEEEE........FFFFF.........",
 "................................................................BBB..................................",
 ".....................................................................................................",
 ".....................................................................................................",
 "...KKKKKKKKKKKKKKKK......LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL...........................",
 "...K..............K..LLLLLL.............................................LL.....MMMMMMM......MMMMMMM..",
 "...K..LLLLLLLLLL..K..L...LL...N.N...........N.N.....OOO............OO...LL...........MMMMMMMM........",
 "...K..L........L..K..L...LL..NN.N.N.......N.N.NN......OOO..........OO...LL.........MMMMMMMMMMMM......",
 "...K..L..KKKK..L..K..L...LL..NN.N.N.N...N.N.N.NN........OOO........OO...LL......MMMM.........MMMM....",
 "...K..L..K.....L..K..L...LL...NNN.N.N.N.N.N.NNN...........OOO......OO...LL......MM..PPP...QQQ..MM....",
 "...K..L..K..LLLL..K..L...LL.....NNN.N.N.N.NNN...............OOO....OO...LL......MM..PPP...QQQ..MM....",
 "...K..L..K........K..L...LL.......NNN.N.NNN...................OOO..OO...LL......MM.............MM....",
 "...K..L..KKKKKKKKKK..L...LL.........NNNNN.......................OOOOO...LL......MMMM.........MMMM....",
 "...K..L..............L...LL...........N...........................OOO...LL........MMMMMMMMMMMMM......",
 "...K..LLLLLLLLLLLLLLLL...LL.............................................LL.............MMM...........",
 "KKKK.....................LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL...........................",
 ".....................................................................................................",
 "..........................................................................................RRR........",
 ".....RRRRRRRRRRR........................RRR.............................................RRRRRRR......",
 "RRRRRRRRRRRRRRRRRRRRRRRR.RRRRRRRRRRRRR.RRRRR.R.RRRR....................................RRRRRRRRR.....",
 "..RRRRRRRRRRRRRRRRRR...RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR.................................RRRRRRR.....R",
 "S.........................................RRRRRR...RRRRRR....TTTTTTT...........RR.........RRRRRRRRRRR",
 "..........................................RRRRRR...RRRRRR......TTT...........RRRRRR............RRRR..",
 ".........UUUUUU.......V....V................RRRRRRRRRRRRRR...TTTTTTT.......RRRR...RRRRR.....RRRR.....",
 ".........UU..UU.......VV..VV...................RRRRRRR...RRRR............RRRR........RRRRRRRRR.......",
 ".........U....U.......VVVVVV................................RRRRRRRRRRRRRR...............RR..RRR....W"
};

/// This function is used to generate a line list and its corresponding Blob indices
/// given a correctly formatted input test 'image'. The test image should be encoded
/// as a vector of strings with one character per pixel. A '.' character is used
/// to indicate a blank pixel and an uppercase letter is used to denote a pixel
/// that is part of a line. The letter used should correspond to the correct
/// blob index for the line - i.e. 'A' is index 0, 'B' is index 1 and so on.
bool GenerateLinesAndResult(const std::vector< std::string >& i_rImage,
	std::vector< Line >& o_rLines,
	std::vector< unsigned int >& o_rBlobIndices)
{
	for (unsigned int Y = 0; Y < i_rImage.size(); ++Y)
	{
		char PreviousPixelOnRow = '.';
		const std::string& rRow = i_rImage[Y];
		for (unsigned int X = 0; X < rRow.size(); ++X)
		{
			char Pixel = rRow[X];
			// check test image does not contain any invalid characters
			if (Pixel != '.' && (Pixel < 'A' || Pixel > 'Z'))
			{
				std::cout << "illegal image - Test harness supports only images\
                      comprising the characters '.' and [A - Z]" << std::endl;
				return false;
			}
			// if the pixel character is not '.' then we must start or extend a line
			if (Pixel != '.')
			{
				if (PreviousPixelOnRow == '.')
				{
					o_rBlobIndices.push_back(static_cast<unsigned int>(Pixel - 'A'));

					Line NewLine;
					NewLine.m_X = X;
					NewLine.m_Y = Y;
					NewLine.m_Length = 1;
					o_rLines.push_back(NewLine);
				}
				else if (Pixel == PreviousPixelOnRow)
				{
					++o_rLines.back().m_Length;
				}
				else
				{
					std::cout << "illegal image - Touching lines on row " << Y << std::endl;
					return false;
				}
			}
			PreviousPixelOnRow = Pixel;
		}
	}
	return true;
}

/// Renders a line list and its blob indices to stdout as an ascii image.
/// The size of the image is determined by the maximum pixel coordinates
/// used by the lines. Blank pixels are printed as a '.' character and
/// pixels that are part of lines are printed using the uppercase letter
/// of the alphabet that corresponds to their blob index.
void RenderImage(const std::vector< Line > i_rLines,
	const std::vector< unsigned int >& i_rBlobIndices)
{
	if (i_rLines.empty())
	{
		return;
	}
	if (i_rLines.size() != i_rBlobIndices.size())
	{
		std::cout << "Can't render image - the number of lines (" << i_rLines.size()
			<< ") does not match the number of assigned BlobIndices ("
			<< i_rBlobIndices.size() << ")" << std::endl;
		return;
	}
	unsigned int MaxColumn = 0;
	for (const auto& rLine : i_rLines)
	{
		MaxColumn = std::max(MaxColumn, rLine.m_X + rLine.m_Length - 1);
	}
	std::string BlankLine(MaxColumn + 1, '.');
	std::vector< std::string > Image(i_rLines.back().m_Y + 1, BlankLine);
	for (size_t i = 0; i < i_rLines.size(); ++i)
	{
		char Pixel = static_cast<char>(i_rBlobIndices[i] + 'A');
		const auto& rLine = i_rLines[i];
		auto& rRow = Image[rLine.m_Y];
		for (unsigned int j = 0; j < rLine.m_Length; ++j)
		{
			rRow[rLine.m_X + j] = Pixel;
		}
	}
	for (auto& rRow : Image)
	{
		std::cout << rRow.c_str() << std::endl;
	}
}

int main()
{
	std::vector< Line > SimpleLineList, ComplexLineList;
	std::vector< unsigned int > SimpleGoldBlobIndices, ComplexGoldBlobIndices;
	if (!GenerateLinesAndResult(g_ImageSimple, SimpleLineList, SimpleGoldBlobIndices) ||
		!GenerateLinesAndResult(g_ImageComplex, ComplexLineList, ComplexGoldBlobIndices))
	{
		return -1;
	}

	BlobBuilder Builder;
	std::vector< unsigned int > BlobIndices;

	Builder.Process(SimpleLineList, BlobIndices);
	RenderImage(SimpleLineList, BlobIndices);
	const bool bSimpleTestPassed = BlobIndices == SimpleGoldBlobIndices;
	std::cout << (bSimpleTestPassed ?
		"Simple Image Test Passed!\n" : "Simple Image Test Failed\n") << std::endl;

	Builder.Process(ComplexLineList, BlobIndices);
	RenderImage(ComplexLineList, BlobIndices);
	/*for (int m =0; m < ComplexGoldBlobIndices.size(); m++)
	{
		std::cout << "ComplexGoldBlobIndices : " << ComplexGoldBlobIndices[m] << std::endl;
	}*/
	const bool bComplexTestPassed = BlobIndices == ComplexGoldBlobIndices;
	std::cout << (bComplexTestPassed ?
		"Complex Image Test Passed!\n" : "Complex Image Test Failed\n") << std::endl;

	const bool bPassed = bSimpleTestPassed && bComplexTestPassed;
	return !bPassed;
}
