var merge_count = 0
var sort_count = 0

#merges 2 sorted arrays
function merge(arr1, arr2)
	merge_count++
	var res = []
	while arr1.length() > 0 and arr2.length() > 0
		if arr1[0] < arr2[0]
			res.push(arr1.shift())
		else 
			res.push(arr2.shift())
	
	while arr1.length() > 0
		res.push(arr1.shift())
	while arr2.length() > 0
		res.push(arr2.shift())
	return res

# sorts the given array
function sort(arr)
	if typeof arr != 'array'
		return Array(0)
	if arr.length() <= 1
		return arr

	sort_count++

	var mid = Integer(arr.length() / 2)
	var tmp1 = []
	for i = 0 to mid - 1
		tmp1.push(arr[i])
	var tmp2 = []
	for i = mid to arr.length() - 1
		tmp2.push(arr[i])
	return merge(sort(tmp1), sort(tmp2))

function main()
	var N = readNumber("Enter the number of numbers: ")
	var arr = Array(N)
	print "Enter the numbers: "
	for i = 0 to N-1
		arr[i] = readNumber()
	
	var res = sort(arr)
	printLine "The sorted array is: ", res
	printLine "Number of merges: ", merge_count, "\nNumber of sorts: ", sort_count

# call the main process
main()