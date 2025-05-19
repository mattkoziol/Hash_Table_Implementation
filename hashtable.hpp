//HashTable Class Public Implementations

template <typename K, typename V>
HashTable<K, V>::HashTable(size_t size) {
    currentSize = 0; //starting out at 0
    if (prime_below(size) == 0) { //if lowest prime is zero, set to default capacity
        theLists.resize(default_capacity);
    }
    else { 
        theLists.resize(prime_below(size));
    }
}

//destructor
template <typename K, typename V>
HashTable<K, V>::~HashTable() {
    makeEmpty(); 
}
//contains function
template <typename K, typename V>
bool HashTable<K, V>::contains(const K& k) const {
    auto& whichList = theLists[myhash(k)]; //list of pairs at position k would be at
    for (auto& kv : whichList) { //for each pair in list
        if (kv.first == k) { //if key found in the list
            return true; 
        }
    }
    return false; //if pair not found
}

//match functions
template <typename K, typename V>
bool HashTable<K, V>::match(const std::pair<K, V> &kv) const {
    auto& whichList = theLists[myhash(kv.first)];
    for (auto& kv2 : whichList) { 
        if (kv2.first == kv.first && kv2.second == kv.second) { 
            return true; 
        }
    }
    return false; 
}
//insert pair function
template <typename K, typename V>
bool HashTable<K, V>::insert(const std::pair<K, V> & kv) {
    auto& whichList = theLists[myhash(kv.first)]; 
    auto itr = find(whichList.begin(), whichList.end(), kv); //find iterator to k
    if (itr != whichList.end()) { 
        return false;
    }
    for (auto& kv2 : whichList) { //if key-value pair not found, check if key is in with different value
        if (kv2.first == kv.first) { //if key found then update the value
            kv2.second = kv.second; 
            return true;
        }
    }
    whichList.push_back(kv); //if key not in the list, push back the full key-value pair
    
    if (++currentSize > theLists.size()) { //rehash if needed
        rehash();
    }
    return true;
}

template <typename K, typename V>
bool HashTable<K, V>::insert(std::pair<K, V> && kv) { //move version of insert given r-value
    auto& whichList = theLists[myhash(kv.first)]; 
    auto itr = find(whichList.begin(), whichList.end(), kv); 
    if (itr != whichList.end()) { 
        return false;
    }
    for (auto& kv2 : whichList) { 
        if (kv2.first == kv.first) { 
            kv2.second = std::move(kv.second); 
            return true;
        }
    }
    whichList.push_back(kv); 
    
    if (++currentSize > theLists.size()) { 
        rehash();
    }
    return true;
}

//remove function
template <typename K, typename V>
bool HashTable<K, V>::remove(const K & k) {
    auto& whichList = theLists[myhash(k)];
    auto itr = whichList.begin(); 
    for (auto& kv : whichList) { 
        if (kv.first == k) { 
            whichList.erase(itr); 
            --currentSize;
            return true;
        }
        ++itr; //move the iterator forward
    }
    return false; 
}

//remove all elements
template <typename K, typename V>
void HashTable<K, V>::clear() {
    makeEmpty(); 
}

//load function
template <typename K, typename V>
bool HashTable<K, V>::load(const char *filename) {
    fstream fh(filename);
    K key;
    V value;
    if (fh.is_open()) {
        while (fh >> key >> value) {
            auto& whichList = theLists[myhash(key)]; //list of pairs at position key would be at
            if (contains(key)) { //if key already in table
                cout << "Error: This key already exists" << endl;
            }
            else { //if key does not exist
                pair<K, V> keyVal;
                keyVal.first = key;
                keyVal.second = value;
                insert(keyVal); //insert the pair
            }
        }
        fh.close();
        return true; //if file existed
    }

    return false;
}

//dump implementation
template <typename K, typename V>
void HashTable<K, V>::dump() const {
    int i = 0;
    cout << "In dump, in total there are " << theLists.size() << " rows in the vector." << endl;
    for (auto& L : theLists) { 
        int count = 0;
        cout << "v[" << i << "]: ";
        for (auto& kv : L) { 
            if (count > 0) { 
                cout << ":";
            }
            cout << kv.first << " " << kv.second;
            ++count;
        }
        cout << endl;
        ++i;
    }
}

//size getter
template <typename K, typename V>
size_t HashTable<K, V>::size() const {
    return currentSize;
}

//write to file implementation
template <typename K, typename V>
bool HashTable<K, V>::write_to_file(const char *filename) const {
    fstream fh(filename);
    if (fh.is_open()) {
        for (auto& L : theLists) { 
            int count = 0;
            for (auto& kv : L) { 
                if (count > 0) { 
                    fh << ": ";
                }
                fh << kv.first << " " << kv.second;
                ++count;
            }
            fh << endl;
        }
        fh.close();
        return true;
    }
    return false;
}

//**Private Member Functions**


//make empty function
template <typename K, typename V>
void HashTable<K, V>::makeEmpty() {
    for (auto& L : theLists) { //for each list in the vector
        L.clear();
    }
    currentSize = 0;
}

//rehashing to new table
template <typename K, typename V>
void HashTable<K, V>::rehash() {
    vector<list<pair<K, V>>> oldLists = theLists; 

    //create new double sized (prime) empty table
    unsigned long newSize = 2 * theLists.size();
    theLists.resize(prime_below(newSize));
    makeEmpty(); 

    //copy old table over
    for (auto& thisList : oldLists) { 
        for (auto& kv : thisList) { 
            insert(std::move(kv));
        }
    }
}

//myhash function
template <typename K, typename V>
size_t HashTable<K, V>::myhash(const K & k) const {
    static hash<K> hf;
    return hf(k) % theLists.size(); 
}

template <typename K, typename V>
unsigned long HashTable<K, V>::prime_below(unsigned long n)
{
    if (n > max_prime)
    {
        std::cerr << "** input too large for prime_below()\n";
        return 0;
    }
    if (n == max_prime)
    {
        return max_prime;
    }
    if (n <= 1)
    {
	    std::cerr << "** input too small \n";
        return 0;
    }

    // now: 2 <= n < max_prime
    std::vector <unsigned long> v (n+1);
    setPrimes(v);
    while (n > 2)
    {
        if (v[n] == 1)
	        return n;
         --n;
    }

    return 2;
}

template <typename K, typename V>
void HashTable<K, V>::setPrimes(vector<unsigned long> & vprimes)
{
    int i = 0;
    int j = 0;

    vprimes[0] = 0;
    vprimes[1] = 0;
    int n = vprimes.capacity();

    for (i = 2; i < n; ++i)
        vprimes[i] = 1;

    for( i = 2; i*i < n; ++i)
    {
        if (vprimes[i] == 1)
            for(j = i + i ; j < n; j += i)
                vprimes[j] = 0;
    }
}