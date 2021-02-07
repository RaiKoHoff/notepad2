#include "EditLexer.h"
#include "EditStyleX.h"

static KEYWORDLIST Keywords_Kotlin = {{
//++Autogenerated -- start of section automatically generated
"abstract actual annotation as break by catch class companion const constructor continue crossinline "
"data delegate do dynamic else enum expect external false field file final finally for fun get "
"if import in infix init inline inner interface internal is it lateinit noinline null object open operator out override "
"package param private property protected public receiver reified return sealed set setparam super suspend "
"tailrec this throw true try typealias typeof val var vararg when where while "

, // 1 class
"AbstractCollection AbstractIterator AbstractList "
"AbstractMap AbstractMutableCollection AbstractMutableList AbstractMutableMap AbstractMutableSet AbstractQueue "
"AbstractSequentialList AbstractSet ActionBar Activity AdapterView AlertDialog Any Application "
"Array ArrayAdapter ArrayBlockingQueue ArrayDeque ArrayList Arrays "
"AtomicBoolean AtomicInteger AtomicLong AtomicReference AudioFormat AudioManager AudioRecord AudioTrack "
"Base64 BaseAdapter BigDecimal BigInteger Binder BitSet Bitmap Boolean BooleanArray "
"Buffer BufferedInputStream BufferedOutputStream BufferedReader BufferedWriter Build Bundle Button "
"Byte ByteArray ByteArrayInputStream ByteArrayOutputStream ByteBuffer ByteOrder "
"Calendar Canvas Char CharArray CharArrayReader CharArrayWriter CharBuffer Character Charset CheckBox ChoiceFormat "
"Class ClassLoader Collections Collectors Color "
"ConcurrentHashMap ConcurrentLinkedDeque ConcurrentLinkedQueue Console Constructor Context ContextWrapper CountDownLatch "
"Currency "
"DataInputStream DataOutputStream DatagramPacket DatagramSocket Date DateFormat DecimalFormat DeflaterOutputStream "
"Dialog Dictionary Display Double DoubleArray DoubleBuffer Drawable "
"EOFException EditText Enum EnumMap EnumSet Environment Error EventObject Exception "
"Field File FileDescriptor FileInputStream FileOutputStream FilePermission FileReader FileSystem FileWriter "
"FilterInputStream FilterOutputStream FilterReader FilterWriter Float FloatArray FloatBuffer Format Formatter "
"GZIPInputStream GZIPOutputStream GregorianCalendar GridView "
"Handler HashMap HashSet Hashtable HttpClient HttpCookie HttpRequest HttpURLConnection "
"IOError IOException Image ImageButton ImageView IndexedValue Inet4Address Inet6Address InetAddress InetSocketAddress "
"InflaterInputStream InputStream InputStreamReader Int IntArray IntBuffer Integer Intent IntentFilter "
"JarEntry JarException JarFile JarInputStream JarOutputStream KeyEvent "
"LayoutInflater LinearLayout LinkedHashMap LinkedHashSet LinkedList ListView Locale Long LongArray LongBuffer Looper "
"MappedByteBuffer MatchGroup Matcher Math Matrix Message MessageFormat Method Modifier Module MotionEvent "
"MulticastSocket "
"Nothing Notification Number NumberFormat "
"Object ObjectInputStream ObjectOutputStream Optional OutputStream OutputStreamWriter "
"Package Paint Pair Parcel Pattern PendingIntent PhantomReference "
"PipedInputStream PipedOutputStream PipedReader PipedWriter Point PointF "
"PrintStream PrintWriter PriorityQueue Process ProcessBuilder ProgressBar Properties "
"RadioButton RadioGroup Random "
"Reader Record Rect RectF Reference ReferenceQueue Regex Region RelativeLayout RemoteException Result "
"Runtime RuntimeException "
"Scanner ScrollView SearchView SecurityManager SeekBar Semaphore ServerSocket Service ServiceLoader "
"Short ShortArray ShortBuffer SimpleDateFormat Socket SocketAddress SoftReference Spinner "
"Stack StackView String StringBuffer StringBuilder StringJoiner StringReader StringTokenizer StringWriter System "
"TableLayout TextView Thread ThreadGroup ThreadLocal ThreadPoolExecutor Throwable TimeZone Timer TimerTask "
"Toast ToggleButton TreeMap TreeSet Triple "
"UByte UByteArray UInt UIntArray ULong ULongArray URI URL URLConnection URLDecoder URLEncoder UShort UShortArray UUID "
"Unit "
"Vector View ViewGroup Void WeakHashMap WeakReference Window Writer "
"ZipEntry ZipException ZipFile ZipInputStream ZipOutputStream "

, // 2 interface
"Adapter Annotation Appendable AutoCloseable BaseStream BlockingDeque BlockingQueue ByteChannel "
"Callable Channel CharSequence Cloneable Closeable Collection Collector Comparable Comparator ConcurrentMap Condition "
"DataInput DataOutput Deque DoubleStream Enumeration EventListener Executor Flushable Formattable Function Future "
"Grouping HttpResponse IBinder IInterface IntStream Iterable Iterator Lazy List ListAdapter ListIterator Lock LongStream "
"Map MatchGroupCollection MatchResult Menu MenuItem "
"MutableCollection MutableIterable MutableIterator MutableList MutableListIterator MutableMap MutableSet "
"NavigableMap NavigableSet ObjectInput ObjectOutput OnClickListener Parcelable Path Predicate Queue "
"RandomAccess ReadWriteLock Readable Runnable Serializable Set SortedMap SortedSet Spliterator Stream WebSocket "

, // 3 enumeration
"AnnotationRetention AnnotationTarget DeprecationLevel ElementType LazyThreadSafetyMode RegexOption RetentionPolicy "
"TimeUnit "

, // 4 annotation
"Basic Column Deprecated Documented Entity FunctionalInterface Generated Id Inherited "
"ManagedBean Metadata MustBeDocumented Native NonEmpty NonNull OrderBy OrderColumn Override "
"PostConstruct PreDestroy Priority Readonly Repeatable ReplaceWith Resource Resources Retention "
"SafeVarargs Serial Suppress SuppressWarnings Table Target Transient Version "

, // 5 function
"assert( check( error( print( println( readLine( require( "

, // 6 KDoc
"author constructor exception param property receiver return sample see since suppress throws "

, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
//--Autogenerated -- end of section automatically generated
}};

static EDITSTYLE Styles_Kotlin[] = {
	EDITSTYLE_DEFAULT,
	{ SCE_KOTLIN_WORD, NP2StyleX_Keyword, L"fore:#0000FF" },
	{ SCE_KOTLIN_ANNOTATION, NP2StyleX_Annotation, L"fore:#FF8000" },
	{ SCE_KOTLIN_CLASS, NP2StyleX_Class, L"fore:#0080FF" },
	{ SCE_KOTLIN_INTERFACE, NP2StyleX_Interface, L"bold; fore:#1E90FF" },
	{ SCE_KOTLIN_ENUM, NP2StyleX_Enumeration, L"fore:#FF8000" },
	{ SCE_KOTLIN_FUNCTION, NP2StyleX_Function, L"fore:#A46000" },
	{ MULTI_STYLE(SCE_KOTLIN_COMMENTBLOCK, SCE_KOTLIN_COMMENTLINE, 0, 0), NP2StyleX_Comment, L"fore:#608060" },
	{ MULTI_STYLE(SCE_KOTLIN_COMMENTBLOCKDOC, SCE_KOTLIN_COMMENTLINEDOC, 0, 0), NP2StyleX_DocComment, L"fore:#408040" },
	{ SCE_KOTLIN_COMMENTDOCWORD, NP2StyleX_DocCommentTag, L"fore:#408080" },
	{ SCE_KOTLIN_TASKMARKER, NP2StyleX_TaskMarker, L"bold; fore:#408080" },
	{ MULTI_STYLE(SCE_KOTLIN_CHARACTER, SCE_KOTLIN_STRING, 0, 0), NP2StyleX_String, L"fore:#008000" },
	{ MULTI_STYLE(SCE_KOTLIN_RAWSTRING, SCE_KOTLIN_RAWSTRINGSTART, SCE_KOTLIN_RAWSTRINGEND, 0), NP2StyleX_RawString, L"fore:#F08000" },
	{ SCE_KOTLIN_ESCAPECHAR, NP2StyleX_EscapeSequence, L"fore:#0080C0" },
	{ SCE_KOTLIN_BACKTICKS, NP2StyleX_Backticks, L"fore:#9E4D2A" },
	{ SCE_KOTLIN_LABEL, NP2StyleX_Label, L"fore:#7C5AF3" },
	{ SCE_KOTLIN_NUMBER, NP2StyleX_Number, L"fore:#FF0000" },
	{ SCE_KOTLIN_VARIABLE, NP2StyleX_Variable, L"fore:#9E4D2A" },
	{ MULTI_STYLE(SCE_KOTLIN_OPERATOR, SCE_KOTLIN_OPERATOR2, 0, 0), NP2StyleX_Operator, L"fore:#B000B0" },
};

EDITLEXER lexKotlin = {
	SCLEX_KOTLIN, NP2LEX_KOTLIN,
	EDITLEXER_HOLE(L"Kotlin Source", Styles_Kotlin),
	L"kt; kts",
	&Keywords_Kotlin,
	Styles_Kotlin
};
