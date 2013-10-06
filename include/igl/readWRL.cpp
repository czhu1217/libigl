#include "readWRL.h"
#include <cstdio>

template <typename Scalar, typename Index>
IGL_INLINE bool igl::readWRL(
  const std::string wrl_file_name,
  std::vector<std::vector<Scalar > > & V,
  std::vector<std::vector<Index > > & F)
{
  using namespace std;
  FILE * wrl_file = fopen(wrl_file_name.c_str(),"r");
  if(NULL==wrl_file)
  {
    printf("IOError: %s could not be opened...",wrl_file_name.c_str());
    return false;
  }

  V.clear();
  F.clear();

  char line[1000];
  // Read lines until seeing "point ["
  // treat other lines in file as "comments"
  bool still_comments = true;
  string needle("point [");
  string haystack;
  while(still_comments)
  {
    fgets(line,1000,wrl_file);
    haystack = string(line);
    still_comments = string::npos == haystack.find(needle);
  }

  // read points in sets of 3
  int floats_read = 3;
  double x,y,z;
  while(floats_read == 3)
  {
    floats_read = fscanf(wrl_file," %lf %lf %lf,",&x,&y,&z);
    if(floats_read == 3)
    {
      vector<Scalar > point;
      point.resize(3);
      point[0] = x;
      point[1] = y;
      point[2] = z;
      V.push_back(point);
      //printf("(%g, %g, %g)\n",x,y,z);
    }else if(floats_read != 0)
    {
      printf("ERROR: unrecognized format...\n");
      return false;
    }
  }
  // Read lines until seeing "coordIndex ["
  // treat other lines in file as "comments"
  still_comments = true;
  needle = string("coordIndex [");
  while(still_comments)
  {
    fgets(line,1000,wrl_file);
    haystack = string(line);
    still_comments = string::npos == haystack.find(needle);
  }
  // read F
  int ints_read = 1;
  while(ints_read > 0)
  {
    // read new face indices (until hit -1)
    vector<Index > face;
    while(true)
    {
      // indices are 0-indexed
      int i;
      ints_read = fscanf(wrl_file," %d,",&i);
      if(ints_read > 0)
      {
        if(i>=0)
        {
          face.push_back(i);
        }else
        {
          F.push_back(face);
          break;
        }
      }else
      {
        break;
      }
    }
  }



  fclose(wrl_file);
  return true;
}