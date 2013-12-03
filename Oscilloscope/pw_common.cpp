#include "stdafx.h"
#include "pw_common.h"

_Exception::_Exception(string err)
{
	Message = err;
}

string _Exception::Error() const
{
	return Message;
}

string ExString::DelSpace(string str)
{
	for (PChar it=str.begin(); it!=str.end(); it++)
	{
		if (*it == ' ')
		{
			str.erase(it--);
		}
	}
	return str;
}

void ExString::Splite(StrArray& strd, string str, const StrArray& strc)
{
	int size = strc.size();
	int maxpos = str.length();
	int *npos = new int[size];
	int index = 0;
	for (CPString it=strc.begin(); it!=strc.end(); it++)
	{
		int pos = str.find(*it);
		if (pos == -1)
		{
			pos = maxpos;
		}
		npos[index++] = pos;
	}

	int minpos = npos[0];
	int minindex = 0;
	for (int i = 1; i < size; i++)
	{
		if (npos[i] < minpos)
		{
			minpos = npos[i];
			minindex = i;
		}
	}
	delete [] npos;
	if (minpos != maxpos)
	{
		string temp = str.substr(0, minpos);
		if (temp.length() >= 1)
		{
			strd.push_back(temp);
		}
		str = str.substr(minpos + 1);
		Splite(strd, str, strc);
	} else {
		strd.push_back(str);
	}
}
StrArray ExString::Splite(string str, const StrArray& strc)
{	
	StrArray ret;
	Splite(ret, str, strc);
	return ret;
}

StrArray ExString::SpliteElement(string str, const StrArray& strc)
{	
	StrArray ret;
	
	str = ExString::DelSpace(str);
	while(true)
	{
		int minpos = 0xFFFFFF;
		CPString mit;
		for (CPString it=strc.begin(); it!=strc.end(); it++)
		{	
			int pos = str.find(*it);
			if (pos < minpos && pos != -1)
			{
				minpos = pos;
				mit = it;
			}
						
		}
		if (minpos != 0xFFFFFF)
		{
			string temp = str.substr(0, minpos);
			if (temp.length() >= 1)
			{
				ret.push_back(temp);
					
			}
			ret.push_back(*mit);
			str = str.substr(minpos + mit->length());				
		}else{
			break;
		}
	}	
	if (str.length() > 0)
	{
		ret.push_back(str);
	}

	return ret;
}

string ExString::DelExcept(string str, string exp)
{
	string ret = "";
	while (true)
	{
		int pos = str.find_first_of(exp);
		if (pos == -1)
		{
			break;
		}
		ret += str[pos];
		str = str.substr(pos + 1);
	}
	return ret;
}

string ExString::Trim(string str)
{
	if (str.length() == 0)
		return str;

	while (str[str.length() - 1] == ' ' || str[str.length() - 1] == '\t' )
	{
		str = str.substr(0, str.length() - 1);
	}
	
	if (str.length() == 0)
		return str;
	while ((str[0] == ' ' || str[0] == '\t'))
	{
		if (str.length() > 1)
			str = str.substr(1);
		else
			str = "";
	}	

	return str;
}

string ExString::ToString(const StrArray& strs)
{
	string ret = "";

	for (CPString it = strs.begin(); it != strs.end(); it++)
	{
		ret += "[" + *it + "]";
	}

	return ret;

}

void ExString::Merger(StrArray& des, const StrArray& src)
{
	des.insert(des.end(), src.begin(), src.end());
}

void ExString::UniqueMerger(StrArray& des,const StrArray& src)
{
	des.insert(des.end(), src.begin(), src.end());
	sort(des.begin(), des.end());
	PString iter_end;
	iter_end = unique(des.begin(), des.end());
	des.erase(iter_end, des.end());
}

StrArray ExString::ToStringArray(string src)
{
	StrArray ret;
	for (unsigned int i=0; i<src.length(); i++)
	{
		ret.push_back(string(&src[i]));
	}
	return ret;
}