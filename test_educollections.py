from educollections import ArrayList


def print_list_state(lst):
    print('Size is', lst.size())
    print('Contents are', lst)
    print()


arr = ArrayList(10)
print('Capacity is', arr.capacity())

print_list_state(arr)

for i in range(10):
    print('Prepend', i)
    arr.prepend(i)
print_list_state(arr)

for i in range(10):
    print('Item at index', i, 'is', arr.get(i))
print_list_state(arr)

for i in range(10):
    print('Assign index', i, 'with', 10 - i)
    arr.set(i, 10 - i)
print_list_state(arr)

arr.clear()
print_list_state(arr)

for i in range(10):
    print('Append', i)
    arr.append(i)
print_list_state(arr)

for i in [9, 4, 1, 6, 3, 0, 0, 0, 1, 0]:
    item = arr.remove(i);
    print('Removed', item, 'from index', i)
    print_list_state(arr)

arr.clear()
print_list_state(arr)

for i in range(5):
    print('Append', i)
    arr.append(i)
print_list_state(arr)

for i in [2, 3, 0, 7, 8]:
    print('Insert', i + 10, 'at index', i)
    arr.insert(i, i + 10)
    print_list_state(arr)
