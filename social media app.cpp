#include<iostream>
#include<fstream>
using namespace std;



class Facebook;
class User;
class Page;
class Activity;
class Date;
class Object;
class Comment;
class Post;
class Memory;

class Helper
{
public:
	static int StringLength(char* str)
	{
		int length = 0;
		for (int i = 0; str[i] != '\0'; i++)
		{
			length++;
		}
		return length;
	}
	static void deepcopy(char*& dest, char* src)
	{
		int i = 0;
		for (; src[i] != '\0'; i++)
		{
			dest[i] = src[i];
		}
		dest[i] = '\0';
	}
	static char* GetStringFromBuffer(char* str)
	{
		int length = StringLength(str);
		char* newstr = new char[length + 1];
		deepcopy(newstr, str);
		return newstr;
	}
	static char* concatenate(int num, char* str) {
		int len = StringLength(str);

		int num_copy = num;
		int digits = 0;
		while (num_copy > 0) {
			digits++;
			num_copy /= 10;
		}
		char* result = new char[len + digits + 1];
		int i = 0;
		for (int j = 0; j < len; j++) {
			result[j] = str[j];
		}
		while (num > 0) {
			result[len + digits - i - 1] = '0' + num % 10;
			num /= 10;
			i++;
		}
		result[len + digits] = '\0';
		return result;
	}


};
class Activity {
	int Type;
	char* value;
public:
	Activity()
	{
		value = 0;
	}
	~Activity()
	{
		if (value != 0)
			delete[] value;
	}

	void ReadDataFromFile(ifstream& fin)
	{
		char temp[50];
		fin >>Type;
		fin.getline(temp,50);
		value = Helper::GetStringFromBuffer(temp);

	}
	void Print()
	{
		if (Type == 1)
		{
			cout << " is feeling   ";
		}
		if (Type == 2)
		{
			cout << "is thinking about  ";
		}
		if (Type == 3)
		{
			cout << " is making ";
		}
		if (Type == 4)
		{
			cout << " is celebrating  ";
		}
		cout << value;
	}
};
class Date {
	int day;
	int month;
	int year;
public:
	Date()
	{
		day = 0;
		month = 0;
		year = 0;
	}
	~Date(){}
	static Date currentDate;
	static void CurrentDate(int d, int m, int y)
	{
		currentDate.day = d;
		currentDate.month = m;
		currentDate.year = y;
	}
	bool Compare(Date temp)
	{
		if (temp.day == day || temp.day-1 == day )
		{
			if (temp.month == month)
			{
				return 1;
			}
		}
		return 0;
	}
	static void PrintDate()
	{
		cout << currentDate.day << "/" << currentDate.month << "/" << currentDate.year << endl;
	}
	void SetCurrentDate(int d,int m,int y)
	{
		day = d;
		month = m;
		year= y;

	}
	void ReadDataFromFile(ifstream& fin)
	{
		fin >> day;
		fin >> month;
		fin >> year;
	}
	void Print()
	{
		cout << "(" << day << "/" << month << "/" << year << ")" << endl;
	}
	int DateDifferences(Date rhs)
	{
		if (day==rhs.day && month==rhs.month)
		{
			return rhs.year - year;
		}

	}

};
class Object {
	char* ID;
public:
	~Object()
	{
		if (ID)
		{
			delete[] ID;
		}
	}

	virtual void AddPosttoTimeline(Post*& ptr) = 0;
	virtual void PrintName() = 0;
	virtual	char* getID() = 0;
};
class Comment
{
	char* ID;
	char* text;
	char* postID;
	Object* CommentBy;
	int noOfComments = 0;
public:
	Comment()
	{
		ID = 0;
		text = 0;
		postID = 0;
		CommentBy = 0;
	}
	~Comment()
	{
		if (ID)
			delete[] ID;
		if (text)
			delete[] text;
	}

	Comment(Object* commentBy, char* te)
	{
		text=Helper::GetStringFromBuffer(te);
		CommentBy = commentBy;
		char* str1 = new char[2];
		str1[0] = 'c';
		str1[1] = '\0';
		ID = Helper::concatenate(noOfComments+ 1, (char*)str1);
		noOfComments++;
	}
	void SetValues(char* id, Object* cBy, char* te)
	{
		noOfComments++;
		ID=Helper::GetStringFromBuffer(id);
		text=Helper::GetStringFromBuffer(te);
		CommentBy = cBy;
	}
	void ViewComment()
	{
		cout << "\t\t";
		//  cout << ID<<" ";
		CommentBy->PrintName();
		cout << "   wrote : ";
		cout << text << endl;
	}

};
class Post {
protected:
	char* ID;
	char* Text;
	int noOfLikes=0;
	Date SharedDate;
	Activity* activity;
	Object* SharedBy;
	Object** LikedBy;
	Comment** comments;//think of size
	int noOfComments = 0;
	int noOfPosts = 0;
public:
	Post()
	{
		ID = 0;
		Text = 0;
		activity = 0;
		SharedBy = 0;
		LikedBy = 0;
		comments = 0;
	}
	~Post()
	{

		if (ID)
			delete[] ID;
		if (Text)
			delete[] Text;
		if (activity)
			delete activity;
		SharedBy = 0;
		if (LikedBy)
		{
			delete[] LikedBy;
		}
		noOfLikes = 0;
		if (comments)
		{
			for (int i = 0; i < noOfComments; i++)
			{
				delete comments[i];
			}
			delete[] comments;
		}
		noOfComments = 0;
	}

	Post(const char* te, Object* ptr)
	{
		Text=Helper::GetStringFromBuffer((char*)te);
		
		SharedBy = ptr;
		SharedDate = Date::currentDate;
		char Str[5] = "post";
		ID = Helper::concatenate(noOfPosts + 1, (char*)Str);
		noOfPosts++;
	}
	void setPostCount(int num)
	{
		noOfPosts = num;
	}

	char* getID()
	{
		return ID;
	}
	void AddComment(Comment* ptr)
	{
		comments[noOfComments] = ptr;
		noOfComments++;
	}
	void ReadDataFromFile(ifstream& fin)
	{
		comments = new Comment * [15];
		LikedBy = new Object * [5];
		int noOfActivities = 0;
		char posttype[2];
		char temp[100];
		fin >> posttype;
		fin >> temp;
		ID = Helper::GetStringFromBuffer(temp);
		SharedDate.ReadDataFromFile(fin);
		fin.getline(temp, 50);
		fin.getline(temp,100);
		Text = Helper::GetStringFromBuffer(temp);
		if (posttype[0] == '2')
		{
			activity = new Activity;
			activity->ReadDataFromFile(fin);
		}
	}
	void setSharedBy(Object* ptr)
	{
		SharedBy = ptr;
	}
	void SetLikedBy(Object* ptr)
	{
		LikedBy[noOfLikes] = ptr;
		noOfLikes++;
	}
	Date getDate()
	{
		return SharedDate;
	}
	virtual void Print(bool flag = true, bool flag1 = true)
	{
		cout << "-----------------------------------------------------------------" << endl;
			cout << endl;
			SharedBy->PrintName();
			if (activity)
			{
				activity->Print();
			}
			if (!activity)
			{
				cout << "has shared  ";
			}
			cout << endl;
			cout << "\"" << Text << "\"";
			cout << "....";
			if (flag)
			{
				SharedDate.Print();
			}
			cout << endl;
			if (flag1)
			{
				ViewComments();
			}
	}
	void ViewComments()
	{
		if (noOfComments > 0)
		{
			cout << "-----------------Comments---------------------\n";
		}
		for (int i = 0; i < noOfComments; i++)
		{
			comments[i]->ViewComment();
		}
	}
	void  viewLikedBy()
	{
		for (int i = 0; i < noOfLikes; i++)
		{
			cout << LikedBy[i]->getID() << "   ";
			LikedBy[i]->PrintName();
			cout << endl;
		}
	}
	void AddComment(Object* ptr, char* text)
	{
		Comment* c1 = new Comment(ptr, text);
		comments[noOfComments] = c1;
		noOfComments++;
	}

};
class Memory : public Post {
	Post* originalPost;
public:
	Memory()
	{
		originalPost = 0;
	}
	Memory(const char* text, Object* UserPtr, Post* ptr) :Post(text, UserPtr)
	{
		originalPost = ptr;
	}
	~Memory(){}
	void Print(bool flag = true, bool flag2 = true)
	{
		cout << endl;
		SharedBy->PrintName();
		cout << " ~~~~  shared a memory ~~~~";
		SharedDate.Print();
		cout << Text << endl;
		Date temp = originalPost->getDate();
		cout << "\t(" << temp.DateDifferences(SharedDate);
		cout << " years ago )" << endl;
		if (activity)
		{
			activity->Print();
		}
		ViewComments();
		originalPost->Print(1, 0);
	}

};
class Page :public Object
{
	char* ID;
	char* Title;
	Post** timeline;//maybe
	int noOfPosts = 0;
public:
	Page()
	{
		ID = 0;
		Title = 0;
		timeline = 0;
	}
	~Page()
	{
		if (ID)
			delete ID;
		if (Title)
			delete Title;
		if (timeline)
		{
			for (int i = 0; i < noOfPosts; i++)
			{
				delete timeline[i];
			}
			delete[] timeline;
		}
		noOfPosts = 0;

	}
	void ReadDataFromFile(ifstream& fin)
	{
		timeline = new Post * [20];
		char* temp = new char[50];
		fin >> temp;
		ID = Helper::GetStringFromBuffer(temp);
		fin.getline(temp, 50);
		Title = Helper::GetStringFromBuffer(temp);
	}
	char* getID()
	{
		return ID;
	}
	void Print()
	{
		cout <<ID<<" "<< Title << endl;
	}
	void AddPosttoTimeline(Post*& ptr)
	{
		timeline[noOfPosts] = ptr;
		noOfPosts++;
	}
	void PrintName()
	{
		cout << Title << "   ";
	}
	void PrintPagesLatest()
	{
		for (int i = 0; i < noOfPosts; i++)
		{
			Date PostDate = timeline[i]->getDate();
			if (PostDate.Compare(Date::currentDate))
			{
				timeline[i]->Print(0, 1);
			}
		}
	}
	void DisplayTimeLine()
	{
		for (int i = noOfPosts - 1; i > 0; i--)
		{
			timeline[i]->Print();
			cout << "  ";
		}
	}

};
class User:public Object
{
	int pagecount = 0;
	int friendcount = 0;
	int noOfPosts = 0;
	char* ID;
	char* Fname;
	char* Lname;
	Page** LikedPages;
	User** FriendList;
	Post** timeline;
public:
	User()
	{
		ID = 0;
		Fname = 0;
		Lname = 0;
		LikedPages = 0;
		FriendList = 0;
		timeline = 0;
	}
	~User()
	{
		if (ID != 0)
			delete[] ID;
		if (Fname != 0)
			delete[] Fname;
		if (Lname != 0)
			delete[] Lname;
		if (LikedPages != 0)
		{

			delete[] LikedPages;
		}
		if (FriendList != 0)
		{
			delete[] FriendList;
		}
		if (timeline)
		{
			for (int i = 0; i < noOfPosts; i++)
			{
				delete timeline[i];
			}

			delete[] timeline;
		}

	}
	void PrintDetails()
	{
		cout << "ID: " << ID << "\t";
		cout << "Name: " << Fname << " " << Lname << " " << endl;
	}
	void PrintName()
	{
		cout << Fname << " " << Lname << " ";
	}
	void ViewFriendList()
	{
		for (int i = 0; i < friendcount; i++)
		{
			cout << FriendList[i]->ID << "  ";
			cout << FriendList[i]->Fname << "   " << FriendList[i]->Lname << endl;
		}
	}
	void ViewLikedPages()
	{
		if (LikedPages != 0)
		{
			cout << "Liked Pages:\n";
			for (int i = 0; i < pagecount; i++)
			{
				LikedPages[i]->Print();
			}
		}
		else
		{
			cout << "User has not liked any pages\n";
		}
	}
	void ViewHomePage()
	{
		for (int i = 0; i < friendcount; i++)
		{
			FriendList[i]->PrintLatestFriends();
		}
		for (int i = 0; i < pagecount; i++)
		{
			LikedPages[i]->PrintPagesLatest();
		}
	}
	void PrintLatestFriends()
	{
		{
			for (int i = 0; i < noOfPosts; i++)
			{
				Date PostDate = timeline[i]->getDate();
				if (PostDate.Compare(Date::currentDate))
				{
					timeline[i]->Print(0, 1);
				}
			}
		}
	}
	void ReadDataFromFile(ifstream& fin)
	{
		LikedPages = new Page * [10];
		timeline = new Post * [20];
		char* temp = new char[10];
		fin >> temp;
		ID = Helper::GetStringFromBuffer(temp);
		fin >> temp;
		Fname = Helper::GetStringFromBuffer(temp);
		fin >> temp;
		Lname = Helper::GetStringFromBuffer(temp);
	}
	void LikePage(Page* ptr)
	{
		//LikedPages[pagecount] = new Page;
		LikedPages[pagecount] = ptr;
		pagecount++;
	}
	void allocateUsersSize(int size)
	{
		FriendList = new User * [size];
	}
	void allocatePagesSize(int size)
	{
		LikedPages = new Page * [size];
	}
	char* getID()
	{
		return ID;
	}
	void FriendUser(User* ptr)
	{
		//FriendList[friendcount] = new User;
		FriendList[friendcount] = ptr;
		friendcount++;

	}
	void AddPosttoTimeline(Post*& ptr)
	{
		timeline[noOfPosts] = ptr;
		noOfPosts++;
	}
	void DisplayTimeline()
	{
		for (int i = noOfPosts - 1; i > 0; i--)
		{
			timeline[i]->Print();
			cout << "  ";
		}
	}
	void seeMemory()
	{
		for (int i = 0; i < noOfPosts; i++)
		{
			if (timeline[i]->getDate().Compare(Date::currentDate))
			{
				cout << "On this Day";
				cout << timeline[i]->getDate().DateDifferences(Date::currentDate);
				cout << "Years ago ";
				timeline[i]->Print();
			}
		}
	}
	void SharedMemory(const char* text, Post* ptr)
	{
		if (noOfPosts < 10)
		{
			timeline[noOfPosts] = new Memory(text, this, ptr);
			noOfPosts++;
		}
	}

};

class Facebook
{
	int totalpages;
	int totalusers;
	int totalcomments;
	static int totalposts;
	User** users;
	Page** pages;
	Comment** comments;
	Post** posts;
public:
	Facebook()
	{
		totalpages = 0;
		totalusers = 0;
		totalcomments = 0;
		totalposts = 0;
		users = 0;
		pages = 0;
		comments = 0;
		posts = 0;
		//objects = 0;
	}
	~Facebook()
	{
		if (pages != 0)
		{
			for (int i = 0; i < totalpages; i++)
			{
				if (pages[i] != 0)
				{
					delete pages[i];
				}
			}
			delete[] pages;
		}
		pages = 0;
		if (users != 0)
		{
			for (int i = 0; i < totalusers; i++)
			{
				delete users[i];
			}
			delete[] users;
		}
		users = 0;
		posts = 0;
	}

	void Print()
	{
		for (int i = 0; i < totalusers; i++)
		{
			users[i]->PrintDetails();
			users[i]->ViewFriendList();
			users[i]->ViewLikedPages();
		}
	}
	void Load()
	{
		ifstream fin;

		fin.open("Pages.txt");
		LoadPagesFromFile(fin);
		fin.close();

		fin.open("Users.txt");
		LoadUsersFromFile(fin);
		fin.close();

		fin.open("Posts.txt");
		LoadPostsFromFile(fin);
		fin.close();

		fin.open("Comments.txt");
		LoadCommentsFromFile(fin);
		fin.close();
	}
	void Run()
	{
		Date::CurrentDate(15, 11, 2017);
		cout << "Command:\tSet current System Date 15 11 2017 \n";
		cout << "System Date:\t";
		Date::PrintDate();
		cout << "Command:\t Set Curerent User \"u7\"" << endl;
		User* currentUser = searchUserbyID("u7");
		currentUser->PrintName();
		cout << " successfully set as Current User\n";
		cout << endl << endl;
		cout << "Command:\tView Friends List \n";
		cout << "-----------------------------------------------------------------------------------------\n";
		currentUser->PrintName();
		cout << "-Friend List\n";
		currentUser->ViewFriendList();
		cout << endl << endl;
		cout << "Command:\tView Liked Pages\n";
		cout << "-----------------------------------------------------------------------------------------\n";
		currentUser->PrintName();
		cout << "-Liked Pages\n\n";
		currentUser->ViewLikedPages();
		cout << "-----------------------------------------------------------------------------------------\n";
		cout << endl << endl;
		cout << "Command:\tView Home \n";
		cout << "-----------------------------------------------------------------------------------------\n";
		currentUser->PrintName();
		cout << "-HomePage\n\n";
		currentUser->ViewHomePage();
		cout << endl << endl;
		cout << "Command:\tView Timeline \n";
		cout << "-----------------------------------------------------------------------------------------\n";
		currentUser->PrintName();
		cout << "-TimeLine\n\n";
		currentUser->DisplayTimeline();
		cout << "-----------------------------------------------------------------------------------------\n";
		cout << "Command:\tView LikedList (post5) \n";
		ViewLikedList("post5");
		cout << "-----------------------------------------------------------------------------------------\n";
		cout << "Command:\t Like(post5) \n";
		LikePost("post5", currentUser);
		cout << "-----------------------------------------------------------------------------------------\n";
		cout << "Command:\tView LikedList (post5) \n";
		ViewLikedList("post5");
		cout << "-----------------------------------------------------------------------------------------\n";
		cout << "Command:\tPost Comment(post4,Good Luck For your Result)  \n";
		PostComment("post4", "Good Luck For your Result ", currentUser);
		ViewPost("post4");
		cout << "\n-----------------------------------------------------------------------------------------\n";
		cout << "Command:\tPost Comment(post8,Thanks For The wishes)  \n";
		PostComment("post8", "Thanks For The wishes", currentUser);
		ViewPost("post8");
		cout << "\n-----------------------------------------------------------------------------------------\n";
		cout << "Command:\tSee Your Memories()\n";
		seeMemory(currentUser);
		cout << "Command\t\t	ShareMemory(post10, Never thought I will be specialist in this field...)" << endl;
		ShareMemory("post10", " Never thought I will be specialist in this field...", currentUser);
		cout << "Command:\tView Timeline \n";
		currentUser->PrintName();
		PostComment("post13", "Thanks For The wishes", currentUser);
		cout << "-TimeLine\n\n";
		currentUser->DisplayTimeline();
		cout << "\n-----------------------------------------------------------------------------------------\n";
		cout << "Command:\tView Page(\"p1\" ) \n";
		ViewPage("p1");
		cout << "\n----------------------------------------------------------------------------------------\n";
		cout << "Command:\t Set Curerent User \"u11\"" << endl;
		currentUser = searchUserbyID("u11");
		currentUser->PrintName();
		cout << " successfully set as Current User\n";
		cout << endl << endl;
		currentUser->ViewHomePage();
		currentUser->DisplayTimeline();


	}
	void setPost(Post* ptr)
	{
		Post** newPosts = new Post * [totalposts + 1];
		for (int i = 0; i < totalposts; i++)
		{
			newPosts[i] = posts[i];
		}
		newPosts[totalposts] = ptr;
		delete[]posts;
		posts = newPosts;
	}

	void ViewPage(const char* pageID)
	{
		Page* currentpage = searchPagesbyID(pageID);
		currentpage->DisplayTimeLine();
	}

	void ShareMemory(const char* postID, const char* text, User*& currentUser)
	{
		cout << "\n-----------------------------------------------------------------------------------------\n";
		Post* ptr = SearchPostByID(postID);
		currentUser->SharedMemory(text, ptr);
	}

	void seeMemory(User*& currentuser)
	{
		currentuser->seeMemory();
	}
	void PostComment(const char* ID, const char* text, User*& currentUser)
	{
		Post* ptr = SearchPostByID(ID);
		if (ptr != 0)
		{
			ptr->AddComment(currentUser, (char*)text);
		}
	}

	void LikePost(const char* id, User*& currentUser)
	{
		Post* ptr=SearchPostByID(id);
		ptr->SetLikedBy(currentUser);
	}
	void ViewPost(const char* str)
	{
		Post* ptr = SearchPostByID(str);
		if (ptr != 0)
			ptr->Print();
	}

	void ViewLikedList(const char* str)
	{

		Post* ptr = SearchPostByID(str);
		ptr->viewLikedBy();
	}
	Object* SearchObjectByID(char* temp)
	{
		if (temp[0] == 'u')
		{
			Object* ptr=searchUserbyID(temp);
			return ptr;
		}
		else if (temp[0] == 'p')
		{
			Object* ptr = searchPagesbyID(temp);
			return ptr;
		}
	}
	Post* SearchPostByID(const char* temp)
	{
		bool found = false;
		int k = 0;
		for (int i = 0; i < totalpages; i++)
		{
			char* ID = posts[i]->getID();
			for (int j = 0; ID[j] != '\0' || temp[j] != '\0'; j++, k++)
			{
				found = true;
				if (ID[j] != temp[k])
				{
					found = false;
					break;
				}
			}
			k = 0;
			if (found == true)
			{
				return posts[i];
			}
		}
		return 0;

	}
	Page* searchPagesbyID(const char* temp)
	{
		bool found = false;
		int k = 0;
		for (int i = 0; i < totalpages; i++)
		{
			char* ID = pages[i]->getID();
			for (int j = 0; ID[j] != '\0' || temp[j] != '\0'; j++, k++)
			{
				found = true;
				if (ID[j] != temp[k])
				{
					found = false;
					break;
				}
			}
			k = 0;
			if (found == true)
			{
				return pages[i];
			}
		}
		return 0;
	}
	User* searchUserbyID(const char* temp)
	{
		bool found = false;

		int k = 0;
		for (int i = 0; i < totalusers; i++)
		{
			char* ID = users[i]->getID();
			for (int j = 0; ID[j] != '\0' || temp[j] != '\0'; j++, k++)
			{
				found = true;
				if (ID[j] != temp[k])
				{
					found = false;
					break;
				}
			}
			k = 0;
			if (found == true)
			{
				return users[i];
			}
		}
	}
	bool Valid(char* arr)
	{
		if (arr[0] == '-' && arr[1] == '1')
		{
			return false;
		}
		else
		{
			return true;

		}
	}
	void LoadPagesFromFile(ifstream& fin)
	{
		fin >> totalpages;
		pages = new Page * [totalpages];
		for (int i = 0; i < totalpages; i++)
		{
			pages[i] = new Page;
			pages[i]->ReadDataFromFile(fin);
		}
	}
	void LoadUsersFromFile(ifstream& fin)
	{
		int friendcount = 0;
		int count = 0;
		char temp[5];
		fin >> totalusers;
		int* noOfFriends = new int[totalusers];
		users = new User * [totalusers];
		char*** tempFriendIDs = new char** [totalusers];
		for (int i = 0; i < totalusers; i++)
		{
			tempFriendIDs[i] = new char* [10];

			users[i] = new User;
			users[i]->ReadDataFromFile(fin);
			for (int j = 0; j < 10; j++)
			{
				fin >> temp;
				if (Valid(temp) == true)
				{
					tempFriendIDs[i][j] = Helper::GetStringFromBuffer(temp);
					friendcount++;
				}
				else
				{
					break;
				}
			}
			if (friendcount != 0)
			{
				users[i]->allocateUsersSize(friendcount);
			}
			noOfFriends[i] = friendcount;
			friendcount = 0;


			for (int j = 0; j < 10; j++)
			{
				fin >> temp;
				if (Valid(temp) == true)
				{
					Page* ptr = searchPagesbyID(temp);
					if (ptr != 0)
					{
						users[i]->LikePage(ptr);
					}
					count++;
				}
				else
				{
					break;
				}
			}


			count = 0;
		}
		//user association
		for (int i = 0; i < totalusers; i++)
		{
			if (noOfFriends[i] != 0)
			{
				for (int j = 0; j < noOfFriends[i]; j++)
				{
					User* userptr = searchUserbyID(tempFriendIDs[i][j]);
					users[i]->FriendUser(userptr);
				}
			}
		}
	}
	void LoadCommentsFromFile(ifstream&fin)
	{
		char* ID;
		char* PostID;
		char* CommentBy;
		char* text;
		char temp[100];
		comments = new Comment * [20];
		fin >> totalcomments;
		for (int i = 0; i < totalcomments; i++)
		{
			comments[i] = new Comment;
			fin >> temp;
			ID = Helper::GetStringFromBuffer(temp);
			fin >> temp;
			PostID = Helper::GetStringFromBuffer(temp);
			fin >> temp;
			CommentBy= Helper::GetStringFromBuffer(temp);
			fin.getline(temp,100);
			text = Helper::GetStringFromBuffer(temp);
			Post* postptr = SearchPostByID(PostID);
			postptr->AddComment(comments[i]);
			Object* commetBy = SearchObjectByID(CommentBy);
			comments[i]->SetValues(ID, commetBy, text);
		}

	}
	void LoadPostsFromFile(ifstream& fin)
	{
		posts = new Post * [11];
		char* objid;
		char temp[5];
		fin >> totalposts;
		for (int i = 0; i < totalposts; i++)
		{
			posts[i] = new Post;
			if (i == 0)
			{
				posts[i]->setPostCount(totalposts);
			}

			posts[i]->ReadDataFromFile(fin);
			fin >> temp;
			objid = Helper::GetStringFromBuffer(temp);
			Object* sharedBy = SearchObjectByID(objid);
			posts[i]->setSharedBy(sharedBy);
			sharedBy->AddPosttoTimeline(posts[i]);
			for (int j = 0; j < 5; j++)
			{
				fin >> temp;
				if (Valid(temp) == true)
				{
					Object* ptr = SearchObjectByID(temp);
					posts[i]->SetLikedBy(ptr);

				}
				else
				{
					break;
				}
			}
		}
	}
};
Date Date::currentDate = Date();
void main()
{
	Facebook fb;
	fb.Load();
	fb.Run();
}
int Facebook::totalposts = 0;