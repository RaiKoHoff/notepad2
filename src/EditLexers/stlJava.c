#include "EditLexer.h"
#include "EditStyleX.h"

static KEYWORDLIST Keywords_Java = {{
//++Autogenerated -- start of section automatically generated
"abstract assert break case catch class const continue default do else enum extends false final finally for goto "
"if implements import instanceof interface native new non-sealed null "
"package parcelable permits private protected public record return sealed static strictfp super switch synchronized "
"this throw throws transient true try var volatile when while yield "

, // 1 types
"boolean byte char double float int long short void "

, // 2 directive
"exports module open opens provides requires to transitive uses with "

, // 3 class
"AbstractCollection AbstractList AbstractMap AbstractQueue AbstractSequentialList AbstractSet ActionBar Activity "
"AdapterView AlertDialog Application Array ArrayAdapter ArrayBlockingQueue ArrayDeque ArrayList Arrays "
"AtomicBoolean AtomicInteger AtomicLong AtomicReference AudioFormat AudioManager AudioRecord AudioTrack "
"Base64 BaseAdapter BigDecimal BigInteger Binder BitSet Bitmap Boolean "
"Buffer BufferedInputStream BufferedOutputStream BufferedReader BufferedWriter Build Bundle Button "
"Byte ByteArrayInputStream ByteArrayOutputStream ByteBuffer ByteOrder "
"Calendar Canvas CharArrayReader CharArrayWriter CharBuffer Character Charset CheckBox ChoiceFormat Class ClassLoader "
"Collections Collectors Color "
"ConcurrentHashMap ConcurrentLinkedDeque ConcurrentLinkedQueue Console Constructor Context ContextWrapper Copy "
"CountDownLatch Currency "
"DataInputStream DataOutputStream DatagramPacket DatagramSocket Date DateFormat DecimalFormat DeflaterOutputStream "
"Dialog Dictionary Display Double DoubleBuffer Drawable "
"EOFException EditText Enum EnumMap EnumSet Environment Error EventObject Exception "
"Field File FileDescriptor FileInputStream FileOutputStream FilePermission FileReader FileSystem FileWriter "
"FilterInputStream FilterOutputStream FilterReader FilterWriter Float FloatBuffer Format Formatter "
"GZIPInputStream GZIPOutputStream Gradle GregorianCalendar GridView "
"Handler HashMap HashSet Hashtable HttpClient HttpCookie HttpRequest HttpURLConnection "
"IOError IOException Image ImageButton ImageView Inet4Address Inet6Address InetAddress InetSocketAddress "
"InflaterInputStream InputStream InputStreamReader IntBuffer Integer Intent IntentFilter "
"JarEntry JarException JarFile JarInputStream JarOutputStream JavaCompile KeyEvent "
"LayoutInflater LinearLayout LinkedHashMap LinkedHashSet LinkedList ListView Locale Long LongBuffer Looper "
"MappedByteBuffer Matcher Math Matrix Message MessageFormat Method Modifier Module MotionEvent MulticastSocket "
"Notification Number NumberFormat Object ObjectInputStream ObjectOutputStream Optional OutputStream OutputStreamWriter "
"Package Paint Parcel Pattern PendingIntent PhantomReference PipedInputStream PipedOutputStream PipedReader PipedWriter "
"Point PointF PrintStream PrintWriter PriorityQueue Process ProcessBuilder ProgressBar Project Properties "
"RadioButton RadioGroup Random Reader Record Rect RectF Reference ReferenceQueue Region RelativeLayout RemoteException "
"Runtime RuntimeException "
"Scanner Script ScrollView SearchView SecurityManager SeekBar Semaphore ServerSocket Service ServiceLoader Settings "
"Short ShortBuffer SimpleDateFormat Socket SocketAddress SoftReference SourceSet Spinner "
"Stack StackView String StringBuffer StringBuilder StringJoiner StringReader StringTokenizer StringWriter System "
"TableLayout Task TextView Thread ThreadGroup ThreadLocal ThreadPoolExecutor Throwable TimeZone Timer TimerTask "
"Toast ToggleButton TreeMap TreeSet "
"URI URL URLConnection URLDecoder URLEncoder UUID Vector View ViewGroup Void WeakHashMap WeakReference Window Writer "
"ZipEntry ZipException ZipFile ZipInputStream ZipOutputStream "

, // 4 interface
"Adapter Annotation Appendable AutoCloseable BaseStream BlockingDeque BlockingQueue ByteChannel "
"Callable Channel CharSequence Cloneable Closeable Collection Collector Comparable Comparator ConcurrentMap Condition "
"DataInput DataOutput Deque DoubleStream Enumeration EventListener Executor Flushable Formattable Function Future "
"HttpResponse IBinder IInterface IntStream Iterable Iterator List ListAdapter ListIterator Lock LongStream "
"Map MatchResult Menu MenuItem NavigableMap NavigableSet ObjectInput ObjectOutput OnClickListener "
"Parcelable Path Predicate Queue RandomAccess ReadWriteLock Readable Runnable "
"Serializable Set SortedMap SortedSet Spliterator Stream WebSocket "

, // 5 enumeration
"ElementType RetentionPolicy TimeUnit "

, // 6 constant
NULL

, // 7 annotation
"Basic Column Delegate DelegatesTo Deprecated Documented Entity FunctionalInterface Generated Id Inherited ManagedBean "
"Native NonEmpty NonNull OrderBy OrderColumn Override PostConstruct PreDestroy Priority "
"Readonly Repeatable Resource Resources Retention SafeVarargs Serial SuppressWarnings Table Target Transient Version "
"interface "

, // 8 function
NULL

, // 9 Javadoc
"author code deprecated docRoot end exception exclude hidden hide highlight index inheritDoc link linkplain literal "
"param provides replace return see serial serialData serialField since snippet spec start summary systemProperty throws "
"uses value version "

, NULL, NULL, NULL, NULL, NULL
//--Autogenerated -- end of section automatically generated

, // 15 Code Snippet
"for^() if^() switch^() while^() catch^() else^if^() else^{} synchronized^() try^() "
}};

static EDITSTYLE Styles_Java[] = {
	EDITSTYLE_DEFAULT,
	{ SCE_JAVA_WORD, NP2StyleX_Keyword, L"fore:#0000FF" },
	{ SCE_JAVA_WORD2, NP2StyleX_TypeKeyword, L"fore:#0000FF" },
	{ SCE_JAVA_DIRECTIVE, NP2StyleX_Directive, L"fore:#FF8000" },
	{ SCE_JAVA_ANNOTATION, NP2StyleX_Annotation, L"fore:#FF8000" },
	{ SCE_JAVA_CLASS, NP2StyleX_Class, L"fore:#0080FF" },
	{ SCE_JAVA_RECORD, NP2StyleX_Record, L"fore:#0080FF" },
	{ SCE_JAVA_INTERFACE, NP2StyleX_Interface, L"bold; fore:#1E90FF" },
	{ SCE_JAVA_ENUM, NP2StyleX_Enumeration, L"fore:#FF8000" },
	{ SCE_JAVA_FUNCTION_DEFINITION, NP2StyleX_MethodDefinition, L"bold; fore:#A46000" },
	{ SCE_JAVA_FUNCTION, NP2StyleX_Method, L"fore:#A46000" },
	{ SCE_JAVA_CONSTANT, NP2StyleX_Constant, L"fore:#B000B0" },
	{ MULTI_STYLE(SCE_JAVA_COMMENTLINE, SCE_JAVA_COMMENTBLOCK, 0, 0), NP2StyleX_Comment, L"fore:#608060" },
	{ MULTI_STYLE(SCE_JAVA_COMMENTLINEDOC, SCE_JAVA_COMMENTBLOCKDOC, 0, 0), NP2StyleX_DocComment, L"fore:#408040" },
	{ MULTI_STYLE(SCE_JAVA_COMMENTTAGAT, SCE_JAVA_COMMENTTAGHTML, 0, 0), NP2StyleX_DocCommentTag, L"fore:#408080" },
	{ SCE_JAVA_TASKMARKER, NP2StyleX_TaskMarker, L"bold; fore:#408080" },
	{ MULTI_STYLE(SCE_JAVA_CHARACTER, SCE_JAVA_STRING, SCE_JAVA_TEMPLATE, 0), NP2StyleX_String, L"fore:#008000" },
	{ MULTI_STYLE(SCE_JAVA_TRIPLE_STRING, SCE_JAVA_TRIPLE_TEMPLATE, 0, 0), NP2StyleX_TextBlock, L"fore:#F08000" },
	{ SCE_JAVA_ESCAPECHAR, NP2StyleX_EscapeSequence, L"fore:#0080C0" },
	{ SCE_JAVA_FORMAT_SPECIFIER, NP2StyleX_FormatSpecifier, L"fore:#7C5AF3" },
	{ SCE_JAVA_PLACEHOLDER, NP2StyleX_Placeholder, L"fore:#8080FF" },
	{ SCE_JAVA_LABEL, NP2StyleX_Label, L"back:#FFC040" },
	{ SCE_JAVA_NUMBER, NP2StyleX_Number, L"fore:#FF0000" },
	{ MULTI_STYLE(SCE_JAVA_OPERATOR, SCE_JAVA_OPERATOR2, 0, 0), NP2StyleX_Operator, L"fore:#B000B0" },
};

EDITLEXER lexJava = {
	SCLEX_JAVA, NP2LEX_JAVA,
//Settings++Autogenerated -- start of section automatically generated
		LexerAttr_PrintfFormatSpecifier |
		LexerAttr_AngleBracketGeneric,
		TAB_WIDTH_4, INDENT_WIDTH_4,
		(1 << 0) | (1 << 1) | (1 << 2), // class, inner class, method
		SCE_JAVA_FUNCTION_DEFINITION,
		'\\', SCE_JAVA_ESCAPECHAR, SCE_JAVA_FORMAT_SPECIFIER,
		0,
		SCE_JAVA_CHARACTER, 0,
		SCE_JAVA_OPERATOR, SCE_JAVA_OPERATOR2
		, KeywordAttr32(0, KeywordAttr_PreSorted) // keywords
		| KeywordAttr32(1, KeywordAttr_PreSorted) // types
		| KeywordAttr32(2, KeywordAttr_PreSorted) // directive
		| KeywordAttr32(3, KeywordAttr_PreSorted) // class
		| KeywordAttr32(4, KeywordAttr_PreSorted) // interface
		| KeywordAttr32(5, KeywordAttr_PreSorted) // enumeration
		| KeywordAttr64(7, KeywordAttr_NoLexer | KeywordAttr_NoAutoComp) // annotation
		| KeywordAttr64(9, KeywordAttr_NoLexer | KeywordAttr_NoAutoComp) // Javadoc
		, SCE_JAVA_TASKMARKER,
		SCE_JAVA_STRING, SCE_JAVA_CHARACTER,
//Settings--Autogenerated -- end of section automatically generated
	EDITLEXER_HOLE(L"Java Source", Styles_Java),
	L"java; jad; aidl; bsh",
	&Keywords_Java,
	Styles_Java
};
