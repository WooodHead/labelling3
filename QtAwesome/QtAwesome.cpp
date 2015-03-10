/**
 * QtAwesome - use font-awesome (or other font icons) in your c++ / Qt Application
 *
 * MIT Licensed
 *
 * Copyright 2013-2015 - Reliable Bits Software by Blommers IT. All Rights Reserved.
 * Author Rick Blommers
 */

#include "QtAwesome.h"

#include <QDebug>
#include <QFile>
#include <QFontDatabase>



/// The font-awesome icon painter
class QtAwesomeCharIconPainter: public QtAwesomeIconPainter
{
public:
    virtual void paint( QtAwesome* awesome, QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state, const QVariantMap& options  )
    {
        Q_UNUSED(mode);
        Q_UNUSED(state);
        Q_UNUSED(options);

        painter->save();

        // set the correct color
        QColor color = options.value("color").value<QColor>();
        QString text = options.value("text").toString();

        if( mode == QIcon::Disabled ) {
            color = options.value("color-disabled").value<QColor>();
            QVariant alt = options.value("text-disabled");
            if( alt.isValid() ) {
                text = alt.toString();
            }
        } else if( mode == QIcon::Active ) {
            color = options.value("color-active").value<QColor>();
            QVariant alt = options.value("text-active");
            if( alt.isValid() ) {
                text = alt.toString();
            }
        } else if( mode == QIcon::Selected ) {
            color = options.value("color-selected").value<QColor>();
            QVariant alt = options.value("text-selected");
            if( alt.isValid() ) {
                text = alt.toString();
            }
        }
        painter->setPen(color);

        // add some 'padding' around the icon
        int drawSize = qRound(rect.height()*options.value("scale-factor").toFloat());

        painter->setFont( awesome->font(drawSize) );
        painter->drawText( rect, text, QTextOption( Qt::AlignCenter|Qt::AlignVCenter ) );
        painter->restore();
    }

};


//---------------------------------------------------------------------------------------


/// The painter icon engine.
class QtAwesomeIconPainterIconEngine : public QIconEngine
{

public:

    QtAwesomeIconPainterIconEngine( QtAwesome* awesome, QtAwesomeIconPainter* painter, const QVariantMap& options  )
        : awesomeRef_(awesome)
        , iconPainterRef_(painter)
        , options_(options)
    {
    }

    virtual ~QtAwesomeIconPainterIconEngine() {}

    QtAwesomeIconPainterIconEngine* clone() const
    {
        return new QtAwesomeIconPainterIconEngine( awesomeRef_, iconPainterRef_, options_ );
    }

    virtual void paint(QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state)
    {
        Q_UNUSED( mode );
        Q_UNUSED( state );
        iconPainterRef_->paint( awesomeRef_, painter, rect, mode, state, options_ );
    }

    virtual QPixmap pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state)
    {
        QPixmap pm(size);
        pm.fill( Qt::transparent ); // we need transparency
        {
            QPainter p(&pm);
            paint(&p, QRect(QPoint(0,0),size), mode, state);
        }
        return pm;
    }

private:

    QtAwesome* awesomeRef_;                  ///< a reference to the QtAwesome instance
    QtAwesomeIconPainter* iconPainterRef_;   ///< a reference to the icon painter
    QVariantMap options_;                    ///< the options for this icon painter
};


//---------------------------------------------------------------------------------------

/// The default icon colors
QtAwesome::QtAwesome( QObject* parent )
    : QObject( parent )
    , namedCodepoints_()
{
    // initialize the default options
    setDefaultOption( "color", QColor(50,50,50) );
    setDefaultOption( "color-disabled", QColor(70,70,70,60));
    setDefaultOption( "color-active", QColor(10,10,10));
    setDefaultOption( "color-selected", QColor(10,10,10));
    setDefaultOption( "scale-factor", 0.9 );

    setDefaultOption( "text", QVariant() );
    setDefaultOption( "text-disabled", QVariant() );
    setDefaultOption( "text-active", QVariant() );
    setDefaultOption( "text-selected", QVariant() );

    fontIconPainter_ = new QtAwesomeCharIconPainter();

}


QtAwesome::~QtAwesome()
{
    delete fontIconPainter_;
//    delete errorIconPainter_;
    qDeleteAll(painterMap_);
}

/// initializes the QtAwesome icon factory with the given fontname
void QtAwesome::init(const QString& fontname)
{
    fontName_ = fontname;
}


/// a specialized init function so font-awesome is loaded and initialized
/// this method return true on success, it will return false if the fnot cannot be initialized
/// To initialize QtAwesome with font-awesome you need to call this method
bool QtAwesome::initFontAwesome( )
{
    static int fontAwesomeFontId = -1;

    // only load font-awesome once
    if( fontAwesomeFontId < 0 ) {

        // The macro below internally calls "qInitResources_QtAwesome()". this initializes
        // the resource system. For a .pri project this isn't required, but when building and using a
        // static library the resource need to initialized first.
        ///
        // I've checked th qInitResource_* code and calling this method mutliple times shouldn't be any problem
        // (More info about this subject:  http://qt-project.org/wiki/QtResources)
        Q_INIT_RESOURCE(QtAwesome);

        // load the font file
        QFile res(":/fonts/fontawesome-4.3.0.ttf");
        if(!res.open(QIODevice::ReadOnly)) {
            qDebug() << "Font awesome font could not be loaded!";
            return false;
        }
        QByteArray fontData( res.readAll() );
        res.close();

        // fetch the given font
        fontAwesomeFontId = QFontDatabase::addApplicationFontFromData(fontData);
    }

    QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(fontAwesomeFontId);
    if( !loadedFontFamilies.empty() ) {
        fontName_= loadedFontFamilies.at(0);
    } else {
        qDebug() << "Font awesome font is empty?!";
        fontAwesomeFontId = -1; // restore the font-awesome id
        return false;
    }

    // intialize the map
    QHash<QString, int>& m = namedCodepoints_;
    m.insert("adjust",              adjust);
    m.insert("adn",                 adn);
    m.insert("aligncenter",         aligncenter);
    m.insert("alignjustify",        alignjustify);
    m.insert("alignleft",           alignleft);
    m.insert("alignright",          alignright);
    m.insert("ambulance",           ambulance);
    m.insert("anchor",              anchor);
    m.insert("android",             android);
    m.insert("angellist",           angellist);
    m.insert("angledoubledown",     angledoubledown);
    m.insert("angledoubleleft",     angledoubleleft);
    m.insert("angledoubleright",    angledoubleright);
    m.insert("angledoubleup",       angledoubleup);
    m.insert("angledown",           angledown);
    m.insert("angleleft",           angleleft);
    m.insert("angleright",          angleright);
    m.insert("angleup",             angleup);
    m.insert("apple",               apple);
    m.insert("archive",             archive);
    m.insert("areachart",           areachart);
    m.insert("arrowcircledown",     arrowcircledown);
    m.insert("arrowcircleleft",     arrowcircleleft);
    m.insert("arrowcircleodown",    arrowcircleodown);
    m.insert("arrowcircleoleft",    arrowcircleoleft);
    m.insert("arrowcircleoright",   arrowcircleoright);
    m.insert("arrowcircleoup",      arrowcircleoup);
    m.insert("arrowcircleright",    arrowcircleright);
    m.insert("arrowcircleup",       arrowcircleup);
    m.insert("arrowdown",           arrowdown);
    m.insert("arrowleft",           arrowleft);
    m.insert("arrowright",          arrowright);
    m.insert("arrowup",             arrowup);
    m.insert("arrows",              arrows);
    m.insert("arrowsalt",           arrowsalt);
    m.insert("arrowsh",             arrowsh);
    m.insert("arrowsv",             arrowsv);
    m.insert("asterisk",            asterisk);
    m.insert("at",                  at);
    m.insert("automobile",          automobile);
    m.insert("backward",            backward);
    m.insert("ban",                 ban);
    m.insert("bank",                bank);
    m.insert("barchart",            barchart);
    m.insert("barcharto",           barcharto);
    m.insert("barcode",             barcode);
    m.insert("bars",                bars);
    m.insert("bed",                 bed);
    m.insert("beer",                beer);
    m.insert("behance",             behance);
    m.insert("behancesquare",       behancesquare);
    m.insert("bell",                bell);
    m.insert("bello",               bello);
    m.insert("bellslash",           bellslash);
    m.insert("bellslasho",          bellslasho);
    m.insert("bicycle",             bicycle);
    m.insert("binoculars",          binoculars);
    m.insert("birthdaycake",        birthdaycake);
    m.insert("bitbucket",           bitbucket);
    m.insert("bitbucketsquare",     bitbucketsquare);
    m.insert("bitcoin",             bitcoin);
    m.insert("bold",                bold);
    m.insert("bolt",                bolt);
    m.insert("bomb",                bomb);
    m.insert("book",                book);
    m.insert("bookmark",            bookmark);
    m.insert("bookmarko",           bookmarko);
    m.insert("briefcase",           briefcase);
    m.insert("btc",                 btc);
    m.insert("bug",                 bug);
    m.insert("building",            building);
    m.insert("buildingo",           buildingo);
    m.insert("bullhorn",            bullhorn);
    m.insert("bullseye",            bullseye);
    m.insert("bus",                 bus);
    m.insert("buysellads",          buysellads);
    m.insert("cab",                 cab);
    m.insert("calculator",          calculator);
    m.insert("calendar",            calendar);
    m.insert("calendaro",           calendaro);
    m.insert("camera",              camera);
    m.insert("cameraretro",         cameraretro);
    m.insert("car",                 car);
    m.insert("caretdown",           caretdown);
    m.insert("caretleft",           caretleft);
    m.insert("caretright",          caretright);
    m.insert("caretsquareodown",    caretsquareodown);
    m.insert("caretsquareoleft",    caretsquareoleft);
    m.insert("caretsquareoright",   caretsquareoright);
    m.insert("caretsquareoup",      caretsquareoup);
    m.insert("caretup",             caretup);
    m.insert("cartarrowdown",       cartarrowdown);
    m.insert("cartplus",            cartplus);
    m.insert("cc",                  cc);
    m.insert("ccamex",              ccamex);
    m.insert("ccdiscover",          ccdiscover);
    m.insert("ccmastercard",        ccmastercard);
    m.insert("ccpaypal",            ccpaypal);
    m.insert("ccstripe",            ccstripe);
    m.insert("ccvisa",              ccvisa);
    m.insert("certificate",         certificate);
    m.insert("chain",               chain);
    m.insert("chainbroken",         chainbroken);
    m.insert("check",               check);
    m.insert("checkcircle",         checkcircle);
    m.insert("checkcircleo",        checkcircleo);
    m.insert("checksquare",         checksquare);
    m.insert("checksquareo",        checksquareo);
    m.insert("chevroncircledown",   chevroncircledown);
    m.insert("chevroncircleleft",   chevroncircleleft);
    m.insert("chevroncircleright",  chevroncircleright);
    m.insert("chevroncircleup",     chevroncircleup);
    m.insert("chevrondown",         chevrondown);
    m.insert("chevronleft",         chevronleft);
    m.insert("chevronright",        chevronright);
    m.insert("chevronup",           chevronup);
    m.insert("child",               child);
    m.insert("circle",              circle);
    m.insert("circleo",             circleo);
    m.insert("circleonotch",        circleonotch);
    m.insert("circlethin",          circlethin);
    m.insert("clipboard",           clipboard);
    m.insert("clocko",              clocko);
    m.insert("close",               close_);
    m.insert("cloud",               cloud);
    m.insert("clouddownload",       clouddownload);
    m.insert("cloudupload",         cloudupload);
    m.insert("cny",                 cny);
    m.insert("code",                code);
    m.insert("codefork",            codefork);
    m.insert("codepen",             codepen);
    m.insert("coffee",              coffee);
    m.insert("cog",                 cog);
    m.insert("cogs",                cogs);
    m.insert("columns",             columns);
    m.insert("comment",             comment);
    m.insert("commento",            commento);
    m.insert("comments",            comments);
    m.insert("commentso",           commentso);
    m.insert("compass",             compass);
    m.insert("compress",            compress);
    m.insert("connectdevelop",      connectdevelop);
    m.insert("copy",                copy);
    m.insert("copyright",           copyright);
    m.insert("creditcard",          creditcard);
    m.insert("crop",                crop);
    m.insert("crosshairs",          crosshairs);
    m.insert("css3",                css3);
    m.insert("cube",                cube);
    m.insert("cubes",               cubes);
    m.insert("cut",                 cut);
    m.insert("cutlery",             cutlery);
    m.insert("dashboard",           dashboard);
    m.insert("dashcube",            dashcube);
    m.insert("database",            database);
    m.insert("dedent",              dedent);
    m.insert("delicious",           delicious);
    m.insert("desktop",             desktop);
    m.insert("deviantart",          deviantart);
    m.insert("diamond",             diamond);
    m.insert("digg",                digg);
    m.insert("dollar",              dollar);
    m.insert("dotcircleo",          dotcircleo);
    m.insert("download",            download);
    m.insert("dribbble",            dribbble);
    m.insert("dropbox",             dropbox);
    m.insert("drupal",              drupal);
    m.insert("edit",                edit);
    m.insert("eject",               eject);
    m.insert("ellipsish",           ellipsish);
    m.insert("ellipsisv",           ellipsisv);
    m.insert("empire",              empire);
    m.insert("envelope",            envelope);
    m.insert("envelopeo",           envelopeo);
    m.insert("envelopesquare",      envelopesquare);
    m.insert("eraser",              eraser);
    m.insert("eur",                 eur);
    m.insert("euro",                euro);
    m.insert("exchange",            exchange);
    m.insert("exclamation",         exclamation);
    m.insert("exclamationcircle",   exclamationcircle);
    m.insert("exclamationtriangle", exclamationtriangle);
    m.insert("expand",              expand);
    m.insert("externallink",        externallink);
    m.insert("externallinksquare",  externallinksquare);
    m.insert("eye",                 eye);
    m.insert("eyeslash",            eyeslash);
    m.insert("eyedropper",          eyedropper);
    m.insert("facebook",            facebook);
    m.insert("facebookf",           facebookf);
    m.insert("facebookofficial",    facebookofficial);
    m.insert("facebooksquare",      facebooksquare);
    m.insert("fastbackward",        fastbackward);
    m.insert("fastforward",         fastforward);
    m.insert("fax",                 fax);
    m.insert("female",              female);
    m.insert("fighterjet",          fighterjet);
    m.insert("file",                file);
    m.insert("filearchiveo",        filearchiveo);
    m.insert("fileaudioo",          fileaudioo);
    m.insert("filecodeo",           filecodeo);
    m.insert("fileexcelo",          fileexcelo);
    m.insert("fileimageo",          fileimageo);
    m.insert("filemovieo",          filemovieo);
    m.insert("fileo",               fileo);
    m.insert("filepdfo",            filepdfo);
    m.insert("filephotoo",          filephotoo);
    m.insert("filepictureo",        filepictureo);
    m.insert("filepowerpointo",     filepowerpointo);
    m.insert("filesoundo",          filesoundo);
    m.insert("filetext",            filetext);
    m.insert("filetexto",           filetexto);
    m.insert("filevideoo",          filevideoo);
    m.insert("filewordo",           filewordo);
    m.insert("filezipo",            filezipo);
    m.insert("fileso",              fileso);
    m.insert("film",                film);
    m.insert("filter",              filter);
    m.insert("fire",                fire);
    m.insert("fireextinguisher",    fireextinguisher);
    m.insert("flag",                flag);
    m.insert("flagcheckered",       flagcheckered);
    m.insert("flago",               flago);
    m.insert("flash",               flash);
    m.insert("flask",               flask);
    m.insert("flickr",              flickr);
    m.insert("floppyo",             floppyo);
    m.insert("folder",              folder);
    m.insert("foldero",             foldero);
    m.insert("folderopen",          folderopen);
    m.insert("folderopeno",         folderopeno);
    m.insert("font",                font_);
    m.insert("forumbee",            forumbee);
    m.insert("forward",             forward);
    m.insert("foursquare",          foursquare);
    m.insert("frowno",              frowno);
    m.insert("futbolo",             futbolo);
    m.insert("gamepad",             gamepad);
    m.insert("gavel",               gavel);
    m.insert("gbp",                 gbp);
    m.insert("ge",                  ge);
    m.insert("gear",                gear);
    m.insert("gears",               gears);
    m.insert("genderless",          genderless);
    m.insert("gift",                gift);
    m.insert("git",                 git);
    m.insert("gitsquare",           gitsquare);
    m.insert("github",              github);
    m.insert("githubalt",           githubalt);
    m.insert("githubsquare",        githubsquare);
    m.insert("gittip",              gittip);
    m.insert("glass",               glass);
    m.insert("globe",               globe);
    m.insert("google",              google);
    m.insert("googleplus",          googleplus);
    m.insert("googleplussquare",    googleplussquare);
    m.insert("googlewallet",        googlewallet);
    m.insert("graduationcap",       graduationcap);
    m.insert("gratipay",            gratipay);
    m.insert("group",               group);
    m.insert("hsquare",             hsquare);
    m.insert("hackernews",          hackernews);
    m.insert("handodown",           handodown);
    m.insert("handoleft",           handoleft);
    m.insert("handoright",          handoright);
    m.insert("handoup",             handoup);
    m.insert("hddo",                hddo);
    m.insert("header",              header);
    m.insert("headphones",          headphones);
    m.insert("heart",               heart);
    m.insert("hearto",              hearto);
    m.insert("heartbeat",           heartbeat);
    m.insert("history",             history);
    m.insert("home",                home);
    m.insert("hospitalo",           hospitalo);
    m.insert("hotel",               hotel);
    m.insert("html5",               html5);
    m.insert("ils",                 ils);
    m.insert("image",               image);
    m.insert("inbox",               inbox);
    m.insert("indent",              indent);
    m.insert("info",                info);
    m.insert("infocircle",          infocircle);
    m.insert("inr",                 inr);
    m.insert("instagram",           instagram);
    m.insert("institution",         institution);
    m.insert("ioxhost",             ioxhost);
    m.insert("italic",              italic);
    m.insert("joomla",              joomla);
    m.insert("jpy",                 jpy);
    m.insert("jsfiddle",            jsfiddle);
    m.insert("key",                 key);
    m.insert("keyboardo",           keyboardo);
    m.insert("krw",                 krw);
    m.insert("language",            language);
    m.insert("laptop",              laptop);
    m.insert("lastfm",              lastfm);
    m.insert("lastfmsquare",        lastfmsquare);
    m.insert("leaf",                leaf);
    m.insert("leanpub",             leanpub);
    m.insert("legal",               legal);
    m.insert("lemono",              lemono);
    m.insert("leveldown",           leveldown);
    m.insert("levelup",             levelup);
    m.insert("lifebouy",            lifebouy);
    m.insert("lifebuoy",            lifebuoy);
    m.insert("lifering",            lifering);
    m.insert("lifesaver",           lifesaver);
    m.insert("lightbulbo",          lightbulbo);
    m.insert("linechart",           linechart);
    m.insert("link",                link_);
    m.insert("linkedin",            linkedin);
    m.insert("linkedinsquare",      linkedinsquare);
    m.insert("linux",               linux_);
    m.insert("list",                list_);
    m.insert("listalt",             listalt);
    m.insert("listol",              listol);
    m.insert("listul",              listul);
    m.insert("locationarrow",       locationarrow);
    m.insert("lock",                lock);
    m.insert("longarrowdown",       longarrowdown);
    m.insert("longarrowleft",       longarrowleft);
    m.insert("longarrowright",      longarrowright);
    m.insert("longarrowup",         longarrowup);
    m.insert("magic",               magic);
    m.insert("magnet",              magnet);
    m.insert("mailforward",         mailforward);
    m.insert("mailreply",           mailreply);
    m.insert("mailreplyall",        mailreplyall);
    m.insert("male",                male);
    m.insert("mapmarker",           mapmarker);
    m.insert("mars",                mars);
    m.insert("marsdouble",          marsdouble);
    m.insert("marsstroke",          marsstroke);
    m.insert("marsstrokeh",         marsstrokeh);
    m.insert("marsstrokev",         marsstrokev);
    m.insert("maxcdn",              maxcdn);
    m.insert("meanpath",            meanpath);
    m.insert("medium",              medium);
    m.insert("medkit",              medkit);
    m.insert("meho",                meho);
    m.insert("mercury",             mercury);
    m.insert("microphone",          microphone);
    m.insert("microphoneslash",     microphoneslash);
    m.insert("minus",               minus);
    m.insert("minuscircle",         minuscircle);
    m.insert("minussquare",         minussquare);
    m.insert("minussquareo",        minussquareo);
    m.insert("mobile",              mobile);
    m.insert("mobilephone",         mobilephone);
    m.insert("money",               money);
    m.insert("moono",               moono);
    m.insert("mortarboard",         mortarboard);
    m.insert("motorcycle",          motorcycle);
    m.insert("music",               music);
    m.insert("navicon",             navicon);
    m.insert("neuter",              neuter);
    m.insert("newspapero",          newspapero);
    m.insert("openid",              openid);
    m.insert("outdent",             outdent);
    m.insert("pagelines",           pagelines);
    m.insert("paintbrush",          paintbrush);
    m.insert("paperplane",          paperplane);
    m.insert("paperplaneo",         paperplaneo);
    m.insert("paperclip",           paperclip);
    m.insert("paragraph",           paragraph);
    m.insert("paste",               paste);
    m.insert("pause",               pause_);
    m.insert("paw",                 paw);
    m.insert("paypal",              paypal);
    m.insert("pencil",              pencil);
    m.insert("pencilsquare",        pencilsquare);
    m.insert("pencilsquareo",       pencilsquareo);
    m.insert("phone",               phone);
    m.insert("phonesquare",         phonesquare);
    m.insert("photo",               photo);
    m.insert("pictureo",            pictureo);
    m.insert("piechart",            piechart);
    m.insert("piedpiper",           piedpiper);
    m.insert("piedpiperalt",        piedpiperalt);
    m.insert("pinterest",           pinterest);
    m.insert("pinterestp",          pinterestp);
    m.insert("pinterestsquare",     pinterestsquare);
    m.insert("plane",               plane);
    m.insert("play",                play);
    m.insert("playcircle",          playcircle);
    m.insert("playcircleo",         playcircleo);
    m.insert("plug",                plug);
    m.insert("plus",                plus);
    m.insert("pluscircle",          pluscircle);
    m.insert("plussquare",          plussquare);
    m.insert("plussquareo",         plussquareo);
    m.insert("poweroff",            poweroff);
    m.insert("print",               print);
    m.insert("puzzlepiece",         puzzlepiece);
    m.insert("qq",                  qq);
    m.insert("qrcode",              qrcode);
    m.insert("question",            question);
    m.insert("questioncircle",      questioncircle);
    m.insert("quoteleft",           quoteleft);
    m.insert("quoteright",          quoteright);
    m.insert("ra",                  ra);
    m.insert("random",              random_);
    m.insert("rebel",               rebel);
    m.insert("recycle",             recycle);
    m.insert("reddit",              reddit);
    m.insert("redditsquare",        redditsquare);
    m.insert("refresh",             refresh);
    m.insert("remove",              remove_);
    m.insert("renren",              renren);
    m.insert("reorder",             reorder);
    m.insert("repeat",              repeat);
    m.insert("reply",               reply);
    m.insert("replyall",            replyall);
    m.insert("retweet",             retweet);
    m.insert("rmb",                 rmb);
    m.insert("road",                road);
    m.insert("rocket",              rocket);
    m.insert("rotateleft",          rotateleft);
    m.insert("rotateright",         rotateright);
    m.insert("rouble",              rouble);
    m.insert("rss",                 rss);
    m.insert("rsssquare",           rsssquare);
    m.insert("rub",                 rub);
    m.insert("ruble",               ruble);
    m.insert("rupee",               rupee);
    m.insert("save",                save);
    m.insert("scissors",            scissors);
    m.insert("search",              search);
    m.insert("searchminus",         searchminus);
    m.insert("searchplus",          searchplus);
    m.insert("sellsy",              sellsy);
    m.insert("send",                send);
    m.insert("sendo",               sendo);
    m.insert("server",              server);
    m.insert("share",               share);
    m.insert("sharealt",            sharealt);
    m.insert("sharealtsquare",      sharealtsquare);
    m.insert("sharesquare",         sharesquare);
    m.insert("sharesquareo",        sharesquareo);
    m.insert("shekel",              shekel);
    m.insert("sheqel",              sheqel);
    m.insert("shield",              shield);
    m.insert("ship",                ship);
    m.insert("shirtsinbulk",        shirtsinbulk);
    m.insert("shoppingcart",        shoppingcart);
    m.insert("signin",              signin);
    m.insert("signout",             signout);
    m.insert("signal",              signal);
    m.insert("simplybuilt",         simplybuilt);
    m.insert("sitemap",             sitemap);
    m.insert("skyatlas",            skyatlas);
    m.insert("skype",               skype);
    m.insert("slack",               slack);
    m.insert("sliders",             sliders);
    m.insert("slideshare",          slideshare);
    m.insert("smileo",              smileo);
    m.insert("soccerballo",         soccerballo);
    m.insert("sort",                sort);
    m.insert("sortalphaasc",        sortalphaasc);
    m.insert("sortalphadesc",       sortalphadesc);
    m.insert("sortamountasc",       sortamountasc);
    m.insert("sortamountdesc",      sortamountdesc);
    m.insert("sortasc",             sortasc);
    m.insert("sortdesc",            sortdesc);
    m.insert("sortdown",            sortdown);
    m.insert("sortnumericasc",      sortnumericasc);
    m.insert("sortnumericdesc",     sortnumericdesc);
    m.insert("sortup",              sortup);
    m.insert("soundcloud",          soundcloud);
    m.insert("spaceshuttle",        spaceshuttle);
    m.insert("spinner",             spinner);
    m.insert("spoon",               spoon);
    m.insert("spotify",             spotify);
    m.insert("square",              square);
    m.insert("squareo",             squareo);
    m.insert("stackexchange",       stackexchange);
    m.insert("stackoverflow",       stackoverflow);
    m.insert("star",                star);
    m.insert("starhalf",            starhalf);
    m.insert("starhalfempty",       starhalfempty);
    m.insert("starhalffull",        starhalffull);
    m.insert("starhalfo",           starhalfo);
    m.insert("staro",               staro);
    m.insert("steam",               steam);
    m.insert("steamsquare",         steamsquare);
    m.insert("stepbackward",        stepbackward);
    m.insert("stepforward",         stepforward);
    m.insert("stethoscope",         stethoscope);
    m.insert("stop",                stop);
    m.insert("streetview",          streetview);
    m.insert("strikethrough",       strikethrough);
    m.insert("stumbleupon",         stumbleupon);
    m.insert("stumbleuponcircle",   stumbleuponcircle);
    m.insert("subscript",           subscript);
    m.insert("subway",              subway);
    m.insert("suitcase",            suitcase);
    m.insert("suno",                suno);
    m.insert("superscript",         superscript);
    m.insert("support",             support);
    m.insert("table",               table);
    m.insert("tablet",              tablet);
    m.insert("tachometer",          tachometer);
    m.insert("tag",                 tag);
    m.insert("tags",                tags);
    m.insert("tasks",               tasks);
    m.insert("taxi",                taxi);
    m.insert("tencentweibo",        tencentweibo);
    m.insert("terminal",            terminal);
    m.insert("textheight",          textheight);
    m.insert("textwidth",           textwidth);
    m.insert("th",                  th);
    m.insert("thlarge",             thlarge);
    m.insert("thlist",              thlist);
    m.insert("thumbtack",           thumbtack);
    m.insert("thumbsdown",          thumbsdown);
    m.insert("thumbsodown",         thumbsodown);
    m.insert("thumbsoup",           thumbsoup);
    m.insert("thumbsup",            thumbsup);
    m.insert("ticket",              ticket);
    m.insert("times",               times);
    m.insert("timescircle",         timescircle);
    m.insert("timescircleo",        timescircleo);
    m.insert("tint",                tint);
    m.insert("toggledown",          toggledown);
    m.insert("toggleleft",          toggleleft);
    m.insert("toggleoff",           toggleoff);
    m.insert("toggleon",            toggleon);
    m.insert("toggleright",         toggleright);
    m.insert("toggleup",            toggleup);
    m.insert("train",               train);
    m.insert("transgender",         transgender);
    m.insert("transgenderalt",      transgenderalt);
    m.insert("trash",               trash);
    m.insert("trasho",              trasho);
    m.insert("tree",                tree);
    m.insert("trello",              trello);
    m.insert("trophy",              trophy);
    m.insert("truck",               truck);
    m.insert("try",                 fa_try);
    m.insert("fa_try",              fa_try);
    m.insert("tty",                 tty);
    m.insert("tumblr",              tumblr);
    m.insert("tumblrsquare",        tumblrsquare);
    m.insert("turkishlira",         turkishlira);
    m.insert("twitch",              twitch);
    m.insert("twitter",             twitter);
    m.insert("twittersquare",       twittersquare);
    m.insert("umbrella",            umbrella);
    m.insert("underline",           underline);
    m.insert("undo",                undo);
    m.insert("university",          university);
    m.insert("unlink",              unlink_);
    m.insert("unlock",              unlock);
    m.insert("unlockalt",           unlockalt);
    m.insert("unsorted",            unsorted);
    m.insert("upload",              upload);
    m.insert("usd",                 usd);
    m.insert("user",                user);
    m.insert("usermd",              usermd);
    m.insert("userplus",            userplus);
    m.insert("usersecret",          usersecret);
    m.insert("usertimes",           usertimes);
    m.insert("users",               users);
    m.insert("venus",               venus);
    m.insert("venusdouble",         venusdouble);
    m.insert("venusmars",           venusmars);
    m.insert("viacoin",             viacoin);
    m.insert("videocamera",         videocamera);
    m.insert("vimeosquare",         vimeosquare);
    m.insert("vine",                vine);
    m.insert("vk",                  vk);
    m.insert("volumedown",          volumedown);
    m.insert("volumeoff",           volumeoff);
    m.insert("volumeup",            volumeup);
    m.insert("warning",             warning);
    m.insert("wechat",              wechat);
    m.insert("weibo",               weibo);
    m.insert("weixin",              weixin);
    m.insert("whatsapp",            whatsapp);
    m.insert("wheelchair",          wheelchair);
    m.insert("wifi",                wifi);
    m.insert("windows",             windows);
    m.insert("won",                 won);
    m.insert("wordpress",           wordpress);
    m.insert("wrench",              wrench);
    m.insert("xing",                xing);
    m.insert("xingsquare",          xingsquare);
    m.insert("yahoo",               yahoo);
    m.insert("yelp",                yelp);
    m.insert("yen",                 yen);
    m.insert("youtube",             youtube);
    m.insert("youtubeplay",         youtubeplay);
    m.insert("youtubesquare",       youtubesquare);

    return true;
}

void QtAwesome::addNamedCodepoint( const QString& name, int codePoint)
{
    namedCodepoints_.insert( name, codePoint);
}


/// Sets a default option. These options are passed on to the icon painters
void QtAwesome::setDefaultOption(const QString& name, const QVariant& value)
{
    defaultOptions_.insert( name, value );
}


/// Returns the default option for the given name
QVariant QtAwesome::defaultOption(const QString& name)
{
    return defaultOptions_.value( name );
}


// internal helper method to merge to option amps
static QVariantMap mergeOptions( const QVariantMap& defaults, const QVariantMap& override )
{
    QVariantMap result= defaults;
    if( !override.isEmpty() ) {
        QMapIterator<QString,QVariant> itr(override);
        while( itr.hasNext() ) {
            itr.next();
            result.insert( itr.key(), itr.value() );
        }
    }
    return result;
}


/// Creates an icon with the given code-point
/// <code>
///     awesome->icon( icon_group )
/// </code>
QIcon QtAwesome::icon(int character, const QVariantMap &options)
{
    // create a merged QVariantMap to have default options and icon-specific options
    QVariantMap optionMap = mergeOptions( defaultOptions_, options );
    optionMap.insert("text", QString( QChar(static_cast<int>(character)) ) );

    return icon( fontIconPainter_, optionMap );
}



/// Creates an icon with the given name
///
/// You can use the icon names as defined on http://fortawesome.github.io/Font-Awesome/design.html  withour the 'icon-' prefix
/// @param name the name of the icon
/// @param options extra option to pass to the icon renderer
QIcon QtAwesome::icon(const QString& name, const QVariantMap& options)
{
    // when it's a named codepoint
    if( namedCodepoints_.count(name) ) {
        return icon( namedCodepoints_.value(name), options );
    }


    // create a merged QVariantMap to have default options and icon-specific options
    QVariantMap optionMap = mergeOptions( defaultOptions_, options );

    // this method first tries to retrieve the icon
    QtAwesomeIconPainter* painter = painterMap_.value(name);
    if( !painter ) {
        return QIcon();
    }

    return icon( painter, optionMap );
}

/// Create a dynamic icon by simlpy supplying a painter object
/// The ownership of the painter is NOT transfered.
/// @param painter a dynamic painter that is going to paint the icon
/// @param optionmap the options to pass to the painter
QIcon QtAwesome::icon(QtAwesomeIconPainter* painter, const QVariantMap& optionMap )
{
    // Warning, when you use memoryleak detection. You should turn it of for the next call
    // QIcon's placed in gui items are often cached and not deleted when my memory-leak detection checks for leaks.
    // I'm not sure if it's a Qt bug or something I do wrong
    QtAwesomeIconPainterIconEngine* engine = new QtAwesomeIconPainterIconEngine( this, painter, optionMap  );
    return QIcon( engine );
}

/// Adds a named icon-painter to the QtAwesome icon map
/// As the name applies the ownership is passed over to QtAwesome
///
/// @param name the name of the icon
/// @param painter the icon painter to add for this name
void QtAwesome::give(const QString& name, QtAwesomeIconPainter* painter)
{
    delete painterMap_.value( name );   // delete the old one
    painterMap_.insert( name, painter );
}

/// Creates/Gets the icon font with a given size in pixels. This can be usefull to use a label for displaying icons
/// Example:
///
///    QLabel* label = new QLabel( QChar( icon_group ) );
///    label->setFont( awesome->font(16) )
QFont QtAwesome::font( int size )
{
    QFont font( fontName_);
    font.setPixelSize(size);
    return font;
}
