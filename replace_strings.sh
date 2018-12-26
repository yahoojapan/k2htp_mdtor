#!/bin/sh

# 2018-12-26
# A script to automate to replace improper words with proper ones.
#
# Note:
# Apply each ruleset in sequence. Changing the order, you will have unexpected results.
#

#
# Rulesets for *ja.md files.
#
echo "perl -pi -e 's/lang_opp_file:\s+(\S+)ja.html/lang_opp_file: $1.html/g' *ja.md"
perl -pi -e 's/lang_opp_file:\s+(\S+)ja.html/lang_opp_file: $1.html/g' *ja.md
echo "perl -pi -e 's/lang_opp_word: To Japanese/lang_opp_word: To English/g' *ja.md"
perl -pi -e 's/lang_opp_word: To Japanese/lang_opp_word: To English/g' *ja.md
echo "perl -pi -e 's/prev_url:\s(\S+).html/prev_url: $1ja.html/g' *ja.md"
perl -pi -e 's/prev_url:\s(\S+).html/prev_url: $1ja.html/g' *ja.md
echo "perl -pi -e 's/next_url:\s(\S+).html/next_url: $1ja.html/g' *ja.md"
perl -pi -e 's/next_url:\s(\S+).html/next_url: $1ja.html/g' *ja.md
echo "perl -pi -e 's/top_url:\s(\S+).html/top_url: $1ja.html/g' *ja.md"
perl -pi -e 's/top_url:\s(\S+).html/top_url: $1ja.html/g' *ja.md

echo "perl -pi -e 's/arrow_link_(\S+):\s<a\shref=\"(\S+).html\"/arrow_link_$1: <a href=\"$2ja.html\"/g' *ja.md"
perl -pi -e 's/arrow_link_(\S+):\s<a\shref="(\S+).html"/arrow_link_$1: <a href="$2ja.html"/g' *ja.md

perl -pi -e 's/home.html/homeja.html/g' *ja.md
perl -pi -e 's/details.html/detailsja.html/g' *ja.md
perl -pi -e 's/usage.html/usageja.html/g' *ja.md
perl -pi -e 's/build.html/buildja.html/g' *ja.md

echo "perl -pi -e 's|K2HASH|[K2HASH](https://k2hash.antpick.ax)|g' *ja.md"
perl -pi -e 's|K2HASH|[K2HASH](https://k2hash.antpick.ax)|g' *ja.md

perl -pi -e 's|mdtorを|**mdtor**を|g' *ja.md
perl -pi -e 's|mdtorの|**mdtor**の|g' *ja.md
perl -pi -e 's|mdtorは|**mdtor**は|g' *ja.md
perl -pi -e 's|mdtorが|**mdtor**が|g' *ja.md
perl -pi -e 's|mdtorに|**mdtor**に|g' *ja.md
perl -pi -e 's|mdtor設定|**mdtor**設定|g' *ja.md
perl -pi -e 's|k2htp_mdtor（別名mdtor）|**k2htp_mdtor**（別名**mdtor**）|g' *ja.md
perl -pi -e 's|K2HTPMDTOR（別名mdtor）|**K2HTPMDTOR**（別名**mdtor**）|g' *ja.md
perl -pi -e 's|mdtorリポジト|**mdtor**リポジト|g' *ja.md
perl -pi -e 's|詳細ページ|[詳細](https://hiwakaba.github.io/k2htp_mdtor/detailsja.html)ページ|g' *ja.md
perl -pi -e 's|ビルドページ|[ビルド](https://hiwakaba.github.io/k2htp_mdtor/buildja.html)ページ|g' *ja.md
perl -pi -e 's|K2HShm :: EnableTransaction|[K2HShm::EnableTransaction](https://k2hash.antpick.ax/developer.html#-k2hshm-class)|g' *ja.md
perl -pi -e 's|k2h_enable_transaction_paramの2|[k2h_enable_transaction_param](https://k2hash.antpick.ax/developer.html#-transaction-archive-familyc-i--f)の2|g' *ja.md



#
# Final rulesets for *.md files.
#
perl -pi -e 's|https://github.com/hiwakaba/k2htp_mdtor/|https://github.com/yahoojapan/k2htp_mdtor/|g' *.md
perl -pi -e 's|https://github.com/hiwakaba/k2htp_mdtor.git|https://github.com/hiwakaba/k2htp_mdtor.git|g' *.md
perl -pi -e 's|https://hiwakaba.github.io/k2htp_mdtor/|https://k2htpmdtor.antpick.ax/|g' *.md

exit 0
